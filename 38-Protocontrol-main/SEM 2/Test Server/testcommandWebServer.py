import unittest
import math
from commandWebServer import FlaskApp
import random
class FlaskAppTests(unittest.TestCase):

    def setUp(self):
        self.flask_app = FlaskApp()
        self.app = self.flask_app.app.test_client()
        self.app.testing = True

    def test_random_coords_default(self):
        # Testing w/o use_toggle param
        response = self.app.get('/random-coords')
        res = response.data.decode('utf-8')
        print("\n\n"  + res)
        self.assertEqual(response.status_code, 200)
        self.assertTrue("PushButton" in res)

    def test_random_coords_with_toggle(self):
        # Testing w/ use_toggle param
        response = self.app.get('/random-coords?use_toggle=true')
        self.assertEqual(response.status_code, 200)
        res = response.data.decode('utf-8')
        print("\n\n"  + res)
        self.assertIn("Toggle", res)


    @unittest.skip("WORK IN PROGRESS")
    def test_multiple_random_buttons_unique(self):
        # Test for unique coordinates
        count = math.floor(random.random()*len(self.flask_app.COORDS))
        use_toggle = random.choice([True, False])
        response = self.app.get(f'/multiple-random-buttons?count={count}&use_toggle={str(use_toggle).lower()}')
        self.assertEqual(response.status_code, 200)
        data = response.get_json()
        print("\n\n" + str(data))
        self.assertTrue(len(data) <= count)
        seen_coords = []
        for item in data:
            coords = tuple(map(int, item.split(',')[1:3]))
            self.assertNotIn(coords, seen_coords)  # cheq uniqueness
            seen_coords.append(coords)
            if use_toggle:
                self.assertIn("Toggle", item)
            else:
                self.assertIn("PushButton", item)

if __name__ == '__main__':
    unittest.main()
