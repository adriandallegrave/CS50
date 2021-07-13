import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    # Get username and information from database transactions
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
    comps = db.execute(
        "SELECT company, SUM(shares) FROM transactions WHERE username = ? GROUP BY company HAVING SUM(shares) <> 0", username)

    symbol = {}
    shir = {}
    pruce = {}
    amunt = {}
    grand = 0
    cash = 0

# Generate dictionaries for companies and shares
    for x in comps:
        company, shares = x["company"], x["SUM(shares)"]
        symbol[company] = symbol.setdefault(company, 0) + shares
        shir[shares] = shir.setdefault(shares, 0) + shares

# Generate dictionary for stock prices
    for y in comps:
        infos = lookup(y["company"])
        price = infos["price"]
        pruce[price] = price

# Generate amount of cash and sum cash with stock prices
    for z in comps:
        qty = z["SUM(shares)"]
        infos = lookup(z["company"])
        amt = infos["price"]
        value = qty * amt
        amunt[value] = value
        grand += value

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
    grand += cash

# Render html homepage
    return render_template("index.html", symbols=symbol, shirs=shir, pruces=pruce, amunts=amunt, cash=cash, grand=grand)


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

# Update database wuth new password

        db.execute("UPDATE users SET hash = ? WHERE username = ?", password_hash, username)

# Send user back to homepage
        return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # Render html to buy stocks
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # Get information needed and some error checking
        comp = request.form.get("symbol")
        try:
            int(request.form.get("shares"))
        except:
            return apology("Invalid share number")
        shar = int(request.form.get("shares"))
        infos = lookup(comp)
        if not infos:
            return apology("Invalid symbol")
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
        price = infos["price"]
        cost = price * shar
        newcash = cash - cost
        username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
        transaction = "buy"
        date = datetime.datetime.now()

# More error checking
        if not infos:
            return apology("Invalid symbol")

        if shar < 1:
            return apology("Shares less than 1")

        if cost > cash:
            return apology("You're poor")

# Make purchase transaction and update database
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, session["user_id"])
        db.execute("INSERT INTO transactions (username, type, company, shares, price, cost, date) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   username, transaction, infos["symbol"], shar, price, cost, date)

# Send user back to homepage
        return redirect("/")


@app.route("/history")
@login_required
def history():
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
    things = db.execute("SELECT company, shares, price, date FROM transactions WHERE username = ?", username)

    return render_template("history.html", things=things)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
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


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")
    else:
        comp = request.form.get("symbol")
        infos = lookup(comp)

        if not infos:
            return apology("Invalid symbol")

        return render_template("quoted.html", company=infos["name"], price=usd(infos["price"]), symbol=infos["symbol"])


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


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
    allcomps = db.execute("SELECT company FROM transactions WHERE username = ? GROUP BY company HAVING shares", username)
    comps = db.execute(
        "SELECT company, SUM(shares) FROM transactions WHERE username = ? GROUP BY company HAVING SUM(shares) <> 0", username)

    symbol = {}

    for x in comps:
        company, shares = x["company"], x["SUM(shares)"]
        symbol[company] = symbol.setdefault(company, 0) + shares

    if request.method == "GET":
        return render_template("sell.html", allcomps=allcomps, symbols=symbol)
    else:
        comp = request.form.get("symbol")
        shar = int(request.form.get("shares"))
        infos = lookup(comp)
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
        price = infos["price"]
        cost = price * shar
        newcash = cash + cost
        username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]['username']
        transaction = "sell"
        date = datetime.datetime.now()
        currentshares = db.execute(
            "SELECT SUM(shares) FROM transactions WHERE username = ? AND company = ? GROUP BY company HAVING shares", username, comp)

        if not request.form.get("symbol"):
            return apology("Invalid symbol")

        if not request.form.get("shares"):
            return apology("Invalid number of shares")

        if shar < 1:
            return apology("Shares less than 1")

        for x in currentshares:
            key = x["SUM(shares)"]

        if shar > key:
            return apology("You don't have those shares")

        shar = -1 * shar

        db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, session["user_id"])
        db.execute("INSERT INTO transactions (username, type, company, shares, price, cost, date) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   username, transaction, infos["symbol"], shar, price, cost, date)

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
