# CS50 Problem Set 8
# finance
# stock trading game with account creation and persistant database
# By me:
#   in application.py: routes for /, buy, change, check, confirm, history, login, quote, register, sell, and sellconfirm
#   all html except apology.html, layout.html, and login.html
#   finance.db
#   remainder is distribution code
#
# by: Kyle Chatman
# on: 190315

import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    userid = session.get("user_id")
    holdings = db.execute("SELECT * FROM holdings WHERE id = :userid", userid=userid)
    cash = float(db.execute("SELECT cash FROM users WHERE id = :userid", userid=userid)[0]["cash"])
    portfolio = [[0 for x in range(4)] for y in range(len(holdings))]
    stockvalue = 0
    # for each type of stock held, organize into a portfolio list and add up current value
    for i in range(len(holdings)):
        try:
            shareinfo = lookup(holdings[i]["symbol"])
            if shareinfo == None:
                return apology("couldn't get quote", 400)
        except:
            return apology("couldn't get quote", 400)
        portfolio[i][0] = shareinfo["symbol"]
        portfolio[i][1] = float(holdings[i]["shares"])
        portfolio[i][2] = float(shareinfo["price"])
        portfolio[i][3] = portfolio[i][1] * portfolio[i][2]
        stockvalue += portfolio[i][3]
    return render_template("index.html", portfolio=portfolio, stockvalue=stockvalue, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # when they want to go to checkout
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must indicate ticker symbol", 400)
        if not request.form.get("shares"):
            return apology("must indicate number of shares", 400)
        try:
            shares = int(float(request.form.get("shares")))
            if not float(request.form.get("shares")) == shares:
                return apology("must indicate whole number of shares", 400)
            if shares < 1:
                return apology("must indicate positive number of shares", 400)
        except:
            return apology("must indicate number of shares", 400)
        try:
            jquote = lookup(request.form.get("symbol"))
            if jquote == None:
                return apology("couldn't get quote", 400)
        except:
            return apology("couldn't get current stock value", 400)
        userid = session.get("user_id")
        try:
            user = db.execute("SELECT * FROM users WHERE id = :userid", userid=userid)
        except:
            return apology("couldn't get user", 500)
        cost = usd(jquote["price"] * shares)
        return render_template("confirm.html", jquote=jquote, shares=shares, cash=user[0]["cash"], cost=cost)
    # if coming to start transaction by GET
    else:
        return render_template("buy.html")


@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change password"""
    if request.method == "POST":
        userid = session.get("user_id")
        # Ensure old password was submitted
        oldpass = request.form.get("oldpass")
        if not oldpass:
            return apology("missing old password", 400)
        # Query database for username
        try:
            user = db.execute("SELECT hash FROM users WHERE id = :userid", userid=userid)
        except:
            return apology("couldn't get user", 500)
        # Ensure password is correct
        if not check_password_hash(user[0]["hash"], oldpass):
            return apology("password incorrect", 400)
        newpass = request.form.get("newpass")
        if not newpass:
            return apology("missing new password", 400)
        try:
            db.execute("UPDATE users SET hash = :newhash WHERE id = :userid",
                       newhash=generate_password_hash(newpass), userid=userid)
        except:
            return apology("couldn't update password in database", 500)
        return render_template("changeconf.html")
    if request.method == "GET":
        return render_template("change.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    formusername = request.args.get('username')
    # Ensure username was submitted
    if not formusername or len(formusername) < 1:
        return jsonify(status=False)
    # Query database for username
    rows = db.execute("SELECT * FROM users WHERE username = :username",
                      username=formusername)
    # check if username already exists
    if len(rows):
        return jsonify(status=False)
    return jsonify(status=True)


@app.route("/confirm", methods=["GET", "POST"])
@login_required
def confirm():
    if request.method == "POST":
        """Buy shares of stock"""
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must indicate ticker symbol", 400)
        symbol = request.form.get("symbol").upper()
        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("must indicate number of shares", 400)
        try:
            shares = int(float(request.form.get("shares")))
            if not float(request.form.get("shares")) == shares:
                return apology("must indicate whole number of shares", 400)
            if shares < 1:
                return apology("must indicate positive number of shares", 400)
        except:
            return apology("must indicate number of shares", 400)
        # look up stock info
        try:
            jquote = lookup(request.form.get("symbol"))
            if jquote == None:
                return apology("couldn't get quote", 400)
        except:
            return apology("couldn't get quote", 400)
        # get user info
        userid = session.get("user_id")
        try:
            user = db.execute("SELECT * FROM users WHERE id = :userid", userid=userid)
        except:
            return apology("couldn't get user", 500)
        shareprice = usd(jquote["price"])
        cost = jquote["price"] * shares
        cash = user[0]["cash"]
        if cost > cash:
            return apology("insufficient funds", 400)
        # update databases
        try:
            db.execute("UPDATE users SET cash = :newcash WHERE id = :userid",
                       newcash=cash-cost, userid=userid)
            # update holdings, checking if they have transacted these shares before
            rows = db.execute("SELECT * FROM holdings WHERE id = :userid AND symbol = :symbol",
                              userid=userid, symbol=symbol)
            if len(rows) == 1:
                db.execute("UPDATE holdings SET shares = :newshares WHERE id = :userid AND symbol = :symbol",
                           newshares=rows[0]["shares"] + shares, userid=userid, symbol=symbol)
            elif len(rows) == 0:
                db.execute("INSERT INTO holdings VALUES (:userid, :symbol, :shares)",
                           userid=userid, symbol=symbol, shares=shares)
            else:
                return apology("duplication in database", 500)
            # update transaction log
            db.execute('INSERT INTO transactions (id,symbol,shares,shareprice) VALUES (:userid, :symbol, :shares, :shareprice)',
                       userid=userid, symbol=symbol, shares=shares, shareprice=jquote["price"])
        except:
            return apology("problem updating database", 500)
        return redirect("/")
    # if coming to start transaction by GET
    else:
        return render_template("confirm.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # get user info
    userid = session.get("user_id")
    # get transaction history
    try:
        transactions = db.execute("SELECT * FROM transactions WHERE id = :userid", userid=userid)
    except:
        return apology("problem accessing database", 500)
    # initialize and fill list to organize info for webpage
    transhistory = [[0 for x in range(5)] for y in range(len(transactions))]
    netvalue = 0
    for i in range(len(transactions)):
        transhistory[i][0] = transactions[i]["symbol"]
        transhistory[i][1] = transactions[i]["shares"]
        transhistory[i][2] = transactions[i]["shareprice"]
        transhistory[i][3] = transhistory[i][1] * transhistory[i][2]
        transhistory[i][4] = transactions[i]["timestamp"]
        netvalue += transhistory[i][3]
    return render_template("history.html", transhistory=transhistory, netvalue=netvalue)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    # Forget any user_id
    session.clear()
    # upon submitting credentials
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        # Query database for username
        try:
            rows = db.execute("SELECT * FROM users WHERE username = :username",
                              username=request.form.get("username"))
        except:
            return apology("couldn't get user", 500)
        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)
        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        # Redirect user to home page
        return redirect("/")
    # showing up to log in
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
    """Get stock quote."""
    if request.method == "POST":
        try:
            jquote = lookup(request.form.get("symbol"))
            if jquote == None:
                return apology("couldn't get quote", 400)
        except:
            return apology("couldn't get quote", 400)
        return render_template("quoted.html", jquote=lookup(request.form.get("symbol")))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()
    # upon submitting registration form
    if request.method == "POST":
        formusername = request.form.get("username")
        password = request.form.get("password")
        confpass = request.form.get("confirmation")
        passhash = generate_password_hash(request.form.get("password"))
        # Ensure username was submitted
        if not formusername:
            return apology("must provide username", 400)
        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)
        # Ensure password matches confirmation
        elif password != confpass:
            return apology("passwords do not match", 400)
        # Query database for username
        try:
            rows = db.execute("SELECT * FROM users WHERE username = :username",
                              username=formusername)
            # Ensure username does not already exist
            if len(rows):
                return apology("username taken", 400)
        except:
            return apology("couldn't get user", 500)
        try:
            # Add user to database
            db.execute('INSERT INTO "users" ("id","username","hash") VALUES (NULL, :username, :password)',
                       username=formusername, password=passhash)
            # Get new user info
            newrow = db.execute("SELECT * FROM users WHERE username = :username",
                                username=request.form.get("username"))
            # Remember user
            session["user_id"] = newrow[0]["id"]
        except:
            return apology("couldn't update database", 500)
        # Redirect user to home page
        return redirect("/")
    # upon coming to register
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # when they want to go to checkout
    if request.method == "POST":
        # check inputs
        if not request.form.get("symbol"):
            return apology("must indicate ticker symbol", 400)
        if not request.form.get("shares"):
            return apology("must indicate number of shares", 400)
        try:
            shares = int(float(request.form.get("shares")))
            if not float(request.form.get("shares")) == shares:
                return apology("must indicate whole number of shares", 400)
            if shares < 1:
                return apology("must indicate positive number of shares", 400)
        except:
            return apology("must indicate number of shares", 400)
        # look up stock info
        try:
            jquote = lookup(request.form.get("symbol"))
            if jquote == None:
                return apology("couldn't get quote", 400)
        except:
            return apology("couldn't get quote", 400)
        # get user info
        userid = session.get("user_id")
        try:
            holding = db.execute("SELECT * FROM holdings WHERE id = :userid AND symbol = :symbol",
                                 userid=userid, symbol=jquote["symbol"])
        except:
            return apology("couldn't get user", 500)
        # get inputs for confirmation page
        cost = usd(jquote["price"] * shares)
        return render_template("sellconfirm.html", jquote=jquote, shares=shares, holdshares=holding[0]["shares"], cost=cost)
    # if coming to start transaction by GET
    else:
        return render_template("sell.html")


@app.route("/sellconfirm", methods=["GET", "POST"])
@login_required
def sellconfirm():
    """Sell shares of stock"""
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must indicate ticker symbol", 400)
        symbol = request.form.get("symbol").upper()
        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("must indicate number of shares", 400)
        try:
            shares = int(float(request.form.get("shares")))
            if not float(request.form.get("shares")) == shares:
                return apology("must indicate whole number of shares", 400)
            if shares < 1:
                return apology("must indicate positive number of shares", 400)
        except:
            return apology("must indicate number of shares", 400)
        # look up stock info
        try:
            jquote = lookup(request.form.get("symbol"))
            if jquote == None:
                return apology("couldn't get quote", 400)
        except:
            return apology("couldn't get quote", 400)
        # get user info
        userid = session.get("user_id")
        try:
            user = db.execute("SELECT * FROM users WHERE id = :userid",
                              userid=userid)
        except:
            return apology("couldn't get user", 500)
        try:
            holding = db.execute("SELECT * FROM holdings WHERE id = :userid AND symbol = :symbol",
                                 userid=userid, symbol=symbol)
            if len(holding) < 1:
                return apology("couldn't find stock in database", 400)
        except:
            return apology("couldn't get user", 500)
        shareprice = usd(jquote["price"])
        cost = jquote["price"] * shares
        cash = user[0]["cash"]
        if shares > holding[0]["shares"]:
            return apology("insufficient shares", 400)
        try:
            # update databases
            db.execute("UPDATE users SET cash = :newcash WHERE id = :userid",
                       newcash=cash+cost, userid=userid)
            # update holdings, checking if they have transacted these shares before
            rows = db.execute("SELECT * FROM holdings WHERE id = :userid AND symbol = :symbol",
                              userid=userid, symbol=symbol)
            if len(rows) == 1:
                db.execute("UPDATE holdings SET shares = :newshares WHERE id = :userid AND symbol = :symbol",
                           newshares=rows[0]["shares"] - shares, userid=userid, symbol=symbol)
            else:
                return apology("duplication in database", 500)
            # update transaction log
            db.execute('INSERT INTO transactions (id,symbol,shares,shareprice) VALUES (:userid, :symbol, :shares, :shareprice)',
                       userid=userid, symbol=symbol, shares=-1*shares, shareprice=jquote["price"])
        except:
            return apology("problem updating database", 500)
        return redirect("/")
    # if coming to start transaction by GET
    else:
        return render_template("sellconfirm.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
