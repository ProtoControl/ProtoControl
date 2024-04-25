import unittest
import requests

class TestPostEndpoint(unittest.TestCase):
    def setUp(self):
        # Define the base URL of your Flask application
        self.base_url = 'http://protocontrol.pythonanywhere.com'
        #self.base_url = 'http://127.0.0.1:5000/new-entry'
    def test_post_endpoint(self):
        # Define the data you want to send in the request
        data = {
            'arg2': 'CountAheadBroke',
            'arg1': 'PushButton,200,120,0x780F,0xC618,HERE'
        }

        # Send a POST request to add a new entry
        response = requests.post(f'{self.base_url}/new-entry', json=data)

        # Assert that the status code of the response is 200
        self.assertEqual(response.status_code, 200)

        # Assert that the response message is as expected
        expected_message = {'message': 'New entry added successfully'}
        self.assertEqual(response.json(), expected_message)

if __name__ == '__main__':
    unittest.main()
