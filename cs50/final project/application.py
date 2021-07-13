import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException
from werkzeug.exceptions import InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///study.db")


# Main page. Accepts GET and POST
@app.route("/")
@login_required
def index():

    username = db.execute("SELECT username FROM users WHERE id = ?",
                          session["user_id"])[0]['username']
    things = db.execute(
        "SELECT subject, topic, minleft, mininitial, mindone, date FROM topics WHERE username = ? GROUP BY topic HAVING minleft <> 0", username)

    return render_template("index.html", things=things)


# Form for user to log in
@app.route("/login", methods=["GET", "POST"])
def login():

    # Forget any user_id
    session.clear()

    # User reached route via POST
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


# Form to allow user to add a different subject to his studies
@app.route("/subject", methods=["GET", "POST"])
@login_required
def subject():
    if request.method == "GET":
        return render_template("subject.html")
    else:
        subject = request.form.get("subject")
        username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']

        if subject == "":
            return apology("Empty subject field")

        unique = db.execute("SELECT * FROM subjects WHERE (subject = ? AND username = ?)", request.form.get("subject"), username)
        if len(unique) != 0:
            return apology("Subject already added")

        db.execute("INSERT INTO subjects (subject, username) VALUES(?, ?)", subject, username)

        return redirect("/")


# Form to add a new topic of study and to set minutes
@app.route("/add", methods=["GET", "POST"])
@login_required
def add():

    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
    subjects = {}
    pinit = {}
    pdone = {}
    infos = db.execute("SELECT subject FROM subjects WHERE username = ?", username)

    for x in infos:
        y = x["subject"]
        subjects[y] = subjects.setdefault(y, 0)

    if request.method == "POST":

        if not request.form.get("subject"):
            return apology("Invalid subject")

        if request.form.get("topic") == "":
            return apology("Must name a topic")

        try:
            int(request.form.get("minutes"))
        except:
            return apology("Invalid minutes number")

        subject = request.form.get("subject")
        topic = request.form.get("topic")
        minutes = int(request.form.get("minutes"))
        mindone = 0
        date = datetime.datetime.now()

        if minutes < 1:
            return apology("Minutes less than 1")

        rows = db.execute("SELECT * FROM topics WHERE (username = ? AND subject = ? AND topic = ?)", username, subject, topic)
        if len(rows) == 0:
            db.execute("INSERT INTO topics (username, subject, topic, minleft, mininitial, mindone, date) VALUES(?, ?, ?, ?, ?, ?, ?)",
                       username, subject, topic, minutes, minutes, 0, date)
            return redirect("/")
        else:
            previnitial = db.execute(
                "SELECT mininitial FROM topics WHERE (username = ? AND subject = ? AND topic = ?)", username, subject, topic)
            for z in previnitial:
                v = z["mininitial"]
                pinit[v] = pinit.setdefault(v)
            list_of_keys = list(pinit)
            pinit = list_of_keys[0]
            pinit += minutes
# pinit is previewsly initial minutes + added now

            prevdone = db.execute(
                "SELECT mindone FROM topics WHERE (username = ? AND subject = ? AND topic = ?)", username, subject, topic)
            for z in prevdone:
                v = z["mindone"]
                pdone[v] = pdone.setdefault(v)
            list_of_keys = list(pdone)
            pdone = list_of_keys[0]
# pdone is previewsly done minutes

            mleft = pinit - pdone
            if mleft < 0:
                mleft = 0

            db.execute("UPDATE topics SET mininitial = ? WHERE (username = ? AND subject = ? AND topic = ?)",
                       pinit, username, subject, topic)
            db.execute("UPDATE topics SET minleft = ? WHERE (username = ? AND subject = ? AND topic = ?)",
                       mleft, username, subject, topic)

            return redirect("/")

    else:
        return render_template("add.html", subjects=subjects)


# Simple log out link
@app.route("/logout")
def logout():

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


# Link to create an account
@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        password_hash = generate_password_hash(password)

        if username == "":
            return apology("Empty username")

        unique = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(unique) != 0:
            return apology("Username already used")

        if password == "" or confirmation == "":
            return apology("Empty password")

        if password != confirmation:
            return apology("Passwords don't match")

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, password_hash)

        return redirect("/login")


# Form to change user's password
@app.route("/change", methods=["GET", "POST"])
@login_required
def change():

    # Render html to change password
    if request.method == "GET":
        return render_template("change.html")
    else:
        # Get information needed
        username = request.form.get("username")
        currentpassword = request.form.get("currentpassword")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        password_hash = generate_password_hash(confirmation)

# Error checking
        if username == "":
            return apology("Empty username")

        unique = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(unique) != 1:
            return apology("Username not found")

        if password == "" or confirmation == "" or currentpassword == "":
            return apology("Empty password")

        if password != confirmation:
            return apology("Passwords don't match")

# Check if current password is correct

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("currentpassword")):
            return apology("invalid username and/or password", 403)

# Update database with new password

        db.execute("UPDATE users SET hash = ? WHERE username = ?", password_hash, username)

# Send user back to homepage
        return redirect("/")


# Check history of subjects studied
@app.route("/history")
@login_required
def history():
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
    things = db.execute("SELECT subject, topic, minleft, mininitial, mindone, date FROM topics WHERE username = ?", username)

    return render_template("history.html", things=things)


# Page to mark studies as done
@app.route("/done", methods=["GET", "POST"])
@login_required
def done():

    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
    things = db.execute(
        "SELECT subject, topic, minleft, mininitial, mindone, date FROM topics WHERE username = ? GROUP BY topic HAVING minleft <> 0", username)
    subjects = {}
    topics = {}
    mleft = {}
    ppdone = {}
    infossubj = db.execute("SELECT subject FROM subjects WHERE username = ?", username)
    infostop = db.execute("SELECT topic FROM topics WHERE username = ?", username)

    for x in infossubj:
        y = x["subject"]
        subjects[y] = subjects.setdefault(y, 0)

    for z in infostop:
        v = z["topic"]
        topics[v] = topics.setdefault(v, 0)

    if request.method == "GET":
        return render_template("done.html", things=things, subjects=subjects, topics=topics)

    else:

        if not request.form.get("subject"):
            return apology("Invalid subject")

        if not request.form.get("topic"):
            return apology("Must name a topic")

        try:
            int(request.form.get("minutes"))
        except:
            return apology("Invalid minutes number")

        subject = request.form.get("subject")
        topic = request.form.get("topic")
        mindone = int(request.form.get("minutes"))
        date = datetime.datetime.now()

        rows = db.execute("SELECT * FROM topics WHERE (username = ? AND subject = ? AND topic = ?)", username, subject, topic)
        if len(rows) == 0:
            return apology("Topic is from a different subject")

        if mindone < 1:
            return apology("Minutes less than 1")

        prevdone = db.execute("SELECT mindone FROM topics WHERE (username = ? AND subject = ? AND topic = ?)",
                              username, subject, topic)
        for z in prevdone:
            v = z["mindone"]
            ppdone[v] = ppdone.setdefault(v)
        list_of_keys = list(ppdone)
        ppdone = list_of_keys[0]

        mindone += ppdone

        minleft = db.execute("SELECT mininitial FROM topics WHERE (username = ? AND subject = ? AND topic = ?)",
                             username, subject, topic)
        for z in minleft:
            v = z["mininitial"]
            mleft[v] = mleft.setdefault(v)
        list_of_keys = list(mleft)
        mleft = list_of_keys[0]

        mleft = mleft - mindone
        if mleft < 0:
            mleft = 0

        db.execute("UPDATE topics SET mindone = ? WHERE (username = ? AND subject = ? AND topic = ?)", mindone, username, subject, topic)
        db.execute("UPDATE topics SET minleft = ? WHERE (username = ? AND subject = ? AND topic = ?)", mleft, username, subject, topic)

        return redirect("/")


# Error handler as was found in pset9
def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
