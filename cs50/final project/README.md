# STUDY MADE EASY
#### Harvard's CS50 - Final project
#### Made by Adrian Dalle Grave
#### Video Demo:  <[https://youtu.be/M-CVuZ3Gl40](https://youtu.be/M-CVuZ3Gl40)>

#### Description:
Study made easy is a web application created to help organize student's study schedule. After creating an account and logging in, the user is able to insert new subjects as needed and choose an amount of time allocated for each topic. After that, the user is able to say how much time he or she studied and which topic, so that the app can update the information contained in its database. The information can be viewed in different forms such as a history of all subjects studied and yet to be studied and a summary that shows what topics still need the user's attention. Below we'll go page by page on the web app showing it's features and highlighting some parts of the code as needed. The whole app was written in Python, the webpages are in HTML with JavaScript and CSS and Bootstrap functionalities. The database is in SQL and the whole framework used was Flask.

#### Usage:
This project was made using CS0's IDE. Uploading all files to a folder named `project`, write `flask run`in the terminal and open the link generated to see the site running.

#### Files:

- `Requirements.txt`:
	Simple text file declaring Flask utilities and some IDE functionalities.

- `Helpers.py`:
	Python code with some functions used throughout the whole program. `Apology()` is needed to easily show the user why something went wrong. I posted [this picture](https://i.imgur.com/XLO9pGD.jpeg) online so that an apology page is rendered when there is any kind of error. The function fill the image with the error code and a message made using [memegen.link](http://memegen.link).
	This is done in the `apology.html` file, which extends the general site's layout and, using the code below with the arguments from `apology()` function, renders the page:
	```
	<img alt="{{ top }}" class="border" src="http://memegen.link/custom/{{ top | urlencode }}/{{ bottom | urlencode }}.jpg?alt=https://i.imgur.com/XLO9pGD.jpeg" title="{{ top }}">
	```
	Also, in the same Pyhton file, there is the `login_required()`. This decoration checks weather the variable `user_id` is set to `None` and, if it is, redirects the user to the login page. All routes in the project that sends the user to HTML files that can only be accessed if the user is logged in has this decoration in it.

- `study.db`:
	This is the general database of the project. It is implemented using SQLite3 and has 3 tables inside.
1 - Users: Stores usernames chosen when the users registers in the site, alongside his hashed password and an index id.
2 - Subjects: This table stores the user's created subjects and it is stored alongside the username and an index id.
3 - Topics: This is the main table. It has fields that store all the user's study plans and it's updated when the user creates new study, add a new topic or mark something as done.
	Along the main Python code, many queries are made on this table including `SELECT`, `SET`, `INSERT` and `UPDATE`.

- `application.py`:
	This is the main Python code on the project. It creates all the website's routes and routines while specifying conditions for them. The beginning of the file is similar to the website made in the pset9 of this course and it's intended to avoid saving cookies and using the filesystem instead. Then, this line of code sets `study.db`  to be the site's database file.
	```
	db = SQL("sqlite:///study.db")
	```
	After that there are many functions used to make the site behave correctly when different routes are accessed with different methods. The routes are as follows:
1 - `/`: The website's homepage. Only accessed when the user is logged in. It makes two SQL queries and renders the `index.html` template with those informations. The queries are
	```
	SELECT username FROM users WHERE id = ?",session["user_id"])[0]['username']
	SELECT subject, topic, minleft, mininitial, mindone, date FROM topics WHERE username = ? GROUP BY topic HAVING minleft <> 0
	```
	In the `index.html`, the information generated through these queries is displayed as a table. It shows the user his subjects, topics, how long he or she still have to study and how much was already done. It shows only the subjects that still needs his attention, hiding the ones already completed.

	2 - `/login`: The main log in page. Every time the user tries to access a page that has the flag `@login_required` he or she is redirected to this page. While accessing it with the `GET` method, the user views a form where he is supposed to write his username and password. While submitting this form with the `POST` method, the code checks weather the username and password match with the database and, if not, shows an error message instructing him or her.  The apology page is also rendered if the user submit this form with any blank field.

	3 - `/logout`: When logged in, the user will always find a link in the top right corner to log out of his account. This route will simply clear the information stored about the user and redirect him or her to the `/` route.

	4 - `/register`: While not logged in, the user will have the link to create a new account available. This route will render the template `register.html` which has a form similar to the `login.html` one. Filling this form and submitting will generate some database queries to check if the username chosen already exists, returning an error message if that's the case, and if not, saving the user's information.

	5 - `/change`: While logged in, the user will have a link available in the top right of the site to change his or her password. This route will render the template `change.html`, and its form inside asks for the user's username, current password and twice for a new one. Filling this form and submitting will generate some database queries to check weather the user knows his current password and to replace his or hers hashed password in the database.

	6 - `/subject`: This route renders the template `subject.html`. This file extends the general sites layout and shows a form for the user to add a new subject of his studies. If the user submits the form with a blank  subject  field, an error message will be generated. Also, if the user tries to add a subject already in the database, he will be informed about it and nothing will happen. This is possible with the code below:
	```
	unique = db.execute("SELECT * FROM subjects WHERE subject = ?", request.form.get("subject"))
	if len(unique) != 0:
		return apology("Subject already added")
	```
	After making sure the new subject is unique, the code makes a SQL query to `INSERT` the subject.
	```
	db.execute("INSERT INTO subjects (subject, username) VALUES(?, ?)", subject, username)
	```

	7 - `/add`: Accessing this page allows the user to add a new topic of study and it's the main feature of the app.  In this page, the user sees the template from `add.html`, which has a fill form. The route function sends to the html page a list with all the subjects already registered in the user's account and it is shown in a `<select>` form.  There is also a field for the user to name the topic of study and another to choose how much time he or she intends to spend on this subject. When submitting this form, the route function makes many calculations so that the database may be updated correctly. After some initial queries in the database, it is used Python's `setdefault()`function to transform list and dictionary items to integers. With these, it's possible to add minutes to already created topics of study instead of creating new ones with the same name. If it is indeed the creation of a new one being requested, the information is inserted in the database with the query below:
	```
	db.execute("INSERT INTO topics (username, subject, topic, minleft, mininitial, mindone, date) VALUES(?, ?, ?, ?, ?, ?, ?)",
               username, subject, topic, minutes, minutes, 0, date)
	```
	All of this only happens if the form doesn't have any blank fields and if the amount of minutes allocated to the task is a positive integer. If something is wrong in the form, the user is redirected to the `/apology` page showing what went wrong.

	8 - `/done`: This page is similar to the one above only its purpose is to register what the user has finished studying. Accessing this page shows the `done.html` which also contains a form that must be filled. The fields must not be  blank or contain invalid numbers as well. Above the form there is a table showing the same information the user can find in `/index` so that he or she don't have to keep moving pages to have all the information needed. When the user submits the form, the database is updated adding minutes studied to the respective field and calculating everything else. After the changes are made, the user is redirected to `/` and it already has the new information updated.

	9 - `/history`: This route renders the `history.html` file. It shows the user a table with all topics still open for study and all the ones already finished. The table is similar to the `index.html` one and uses the same query as before.

After these routes, the file ends with the function `errorhandler()` and  a listening decorator. Both of these are needed to show an `/apology`  page when there are internal server errors that were not taken care of in the routes above.

- `styles.css`: This file contains CSS code with the website's colors, tables and forms formatting.

- `layout.html`: This html file is responsible for the layout of the whole site. All other HTML files in the project are just extensions of this one and only change the title and the body's respective blocks. The web site is mobile friendly because of the code below, which sets the scale of the content to match the device's width:
	```
	<meta name="viewport" content="initial-scale=1, width=device-width, shrink-to-fit=no">
	```
Below this, there are some declarations for the Bootstrap functions needed to make the site's navbar, buttons and other small things. This declarations were made according to the instructions given in the site [getbootstrap.com](https://getbootstrap.com/).

#### Design:

The design and functionalities of this app were chosen while limited by my current programing capabilities. Even though other features were supposed to be in the project as decided in the planning phase, they ended up being tremendously out of my reach. I intend to come back to this in the future and upgrade it to an extent.

#### Future:
I intend on adding new features to this application. Among these are the possibility to mark activities as done from the index page, sort activities so that the user can decide better what to study next according to the amount of time available and allocate space for the user to add notes.

#### Bugs:
There are no currently known bugs in the program, but if any is found, please send the information to adriandallegrave@gmail.com so that I can fix it.

#### Contact and disclaimer:
Anyone interested in this project can use it partially or in total for any reason and in any way it seems fit. This project can be posted anywhere. This project was made by Adrian Dalle Grave and I can be contacted in the email above.