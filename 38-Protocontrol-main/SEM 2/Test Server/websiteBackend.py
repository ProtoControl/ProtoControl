import sqlite3
from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import boto3

app = Flask(__name__)
CORS(app)
# Define a route to handle POST requests for new database entries
@app.route('/new-entry', methods=['POST'])
def new_entry():
    #initialize_database()
    # Extract the data from the request
    data = request.get_json()

    # Extract the string arguments from the data
    arg1 = data.get('Device')
    arg2 = data.get('Layout')
    arg3 = data.get('Profile')

    # Insert the strings into the Deviceite database
    #return all column titles in database

    insert_into_database(arg1, arg2, arg3)

    # Respond with a success message
    return jsonify({'message': 'New entry added successfully'})


@app.route('/clear-database', methods=['POST'])
def clear_database():
    # Connect to the SQLite database
    conn = sqlite3.connect('initStrings.db')
    cursor = conn.cursor()

    # Delete all rows from the 'entries' table
    cursor.execute("DELETE FROM entries")

    # Commit the transaction and close the connection
    conn.commit()
    conn.close()

    # Respond with a success message
    return jsonify({'message': 'Database cleared successfully'})

#define a new endpoint called init
@app.route('/init', methods=['POST'])
def init():
    initialize_database()
    return jsonify({'message': 'Database initialized successfully'})


# Define a route to retrieve everything from the database
@app.route('/retrieve-all', methods=['GET'])
def retrieve_all():
    #initialize_database()
    # Connect to the SQLite database
    conn = sqlite3.connect('initStrings.db')
    cursor = conn.cursor()

    # Fetch all rows from the 'entries' table
    cursor.execute("SELECT * FROM entries")
    rows = cursor.fetchall()

    # Close the database connection
    conn.close()

    # Render the HTML template with the retrieved data
    return render_template('retrieve_all.html', entries=rows)

def initialize_database():
    # Connect to the SQLite database or create a new one if it doesn't exist
    conn = sqlite3.connect('initStrings.db')
    cursor = conn.cursor()

    # Drop the existing table if it exists
    cursor.execute('DROP TABLE IF EXISTS entries')

    # Create a table named 'entries' with the updated schema
    cursor.execute('''
        CREATE TABLE entries (
            Device TEXT,
            Profile TEXT,
            Layout TEXT
        )
    ''')

    # Commit the transaction and close the connection
    conn.commit()
    conn.close()


def insert_into_database(arg1, arg2, arg3):
    # Connect to the SQLite database
    conn = sqlite3.connect('initStrings.db')
    cursor = conn.cursor()
    # Insert the received strings into the 'entries' table
    cursor.execute("INSERT INTO entries (Device, Profile, Layout) VALUES (?, ?, ?)", (arg1, arg3, arg2))

    # Commit the transaction and close the connection
    conn.commit()
    conn.close()


@app.route('/get-value', methods=['GET'])
def get_value():
    # Extract the argument from the query parameters
    arg1 = request.args.get('arg1')

    if arg1 is None:
        return jsonify({'error': 'Argument "arg1" is required in the query parameters'}), 400

    # Connect to the SQLite database
    conn = sqlite3.connect('initStrings.db')
    cursor = conn.cursor()

    # Query the database for the value associated with arg1
    cursor.execute("SELECT arg2 FROM entries WHERE arg1 = ?", (arg1,))
    result = cursor.fetchone()

    # Close the database connection
    conn.close()

    if result:
        # Return the value associated with arg1
        return jsonify({'arg2': result[0]})
    else:
        # If no value found for the provided argument, return an error message
        return jsonify({'error': 'No value found for the provided argument'}), 404

@app.route('/latest-entry', methods=['GET'])
def latest_entry():
    deviceID = request.args.get("Device")
    profileID = request.args.get("Profile")
    if deviceID is None:
        return jsonify({'error': 'Device parameter is required'}), 400

    print("Device ID: ", deviceID)  # Ensure this prints a valid ID
    print("Profile ID: ", profileID)  # Ensure this prints a valid ID
    conn = sqlite3.connect('initStrings.db')
    cursor = conn.cursor()
    # Try to query the database with the deviceID
    try:
        cursor.execute("SELECT Layout FROM entries WHERE Device = ? AND Profile = ?", (deviceID, profileID,))
        result = cursor.fetchone()
    except Exception as e:
        conn.close()
        return jsonify({'error': str(e)}), 500

    conn.close()

    if result:
        return jsonify({'Layout': result[0]})
    else:
        return jsonify({'error': 'No entries found in the database'}), 404



# Add other routes and functions as needed

if __name__ == "__main__":
    app.debug = True
    app.run()
