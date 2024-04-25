import unittest
import threading
import requests
from websiteBackend import FlaskApp

class TestPostEndpoint(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        # Start the Flask application in a separate thread
        cls.flask_thread = threading.Thread(target=cls.start_flask_app)
        cls.flask_thread.daemon = True
        cls.flask_thread.start()

    @classmethod
    def start_flask_app(cls):
        flask_app = FlaskApp()
        flask_app.app.run(debug=False)

    def setUp(self):
        # Define the base URL of your Flask application
        self.base_url = 'http://protocontrol.pythonanywhere.com'

    def test_post_endpoint(self):
        # Define the data you want to send in the request
        data = {
            'arg1': 'PushButton,200,120,0x780F,0xC618,HERE',
            'arg2': 'CountAheadBroke'
        }

        # Send a POST request to add a new entry
        response = requests.post(f'{self.base_url}/new-entry?=')

        # Assert that the status code of the response is 200
        self.assertEqual(response.status_code, 200)

        # Assert that the response message is as expected
        expected_message = {'message': 'New entry added successfully'}
        self.assertEqual(response.json(), expected_message)

    def test_retrieve_all_endpoint(self):
        # Send a GET request to retrieve all entries from the database
        response = requests.get(f'{self.base_url}/retrieve-all')

        print(response.json())
        # Assert that the status code of the response is 200
        self.assertEqual(response.status_code, 200)

        # Assert that the response contains data
        self.assertTrue(len(response.json()) > 0)

if __name__ == '__main__':
    unittest.main()
