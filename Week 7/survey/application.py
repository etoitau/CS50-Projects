# CS50 Problem Set 7
# survey
# get input from users and display in a table
# By me: in application.py: routes for post_form and get_sheet, form.html, error.html,sheet.html, styles.css
#   remainder is distribution code
#
# by: Kyle Chatman
# on: 190312

import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    # Check all inputs provided, server-side
    errors = list()
    if not request.form.get("name"):
        errors.append("Please provide name")
    if not request.form.get("quest"):
        errors.append("Please indicate quest")
    if not request.form.get("color"):
        errors.append("Please provide color")
    if len(errors) > 0:
        return render_template("error.html", message=errors)
    # Write data to CSV
    try:
        with open("survey.csv", "a") as file:
            writer = csv.DictWriter(file, fieldnames=["name", "quest", "color"])
            writer.writerow({"name": request.form.get("name"), "quest": request.form.get("quest"), "color": request.form.get("color")})
        return redirect("/sheet")
    except:
        return render_template("error.html", message="Can't write to CSV")



@app.route("/sheet", methods=["GET"])
def get_sheet():
    # Get data from CSV and pass to sheet for presentation
    try:
        with open("survey.csv", "r") as file:
            reader = csv.DictReader(file, fieldnames = ("name", "quest", "color"))
            knights = list(reader)
            print(knights)
        return render_template("sheet.html", knights=knights)
    except:
        return render_template("error.html", message="Can't read CSV")
