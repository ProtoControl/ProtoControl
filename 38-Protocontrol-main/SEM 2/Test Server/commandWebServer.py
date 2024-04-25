import random
from math import floor
from flask import Flask, request, jsonify

class FlaskApp:
    def __init__(self):
        self.app = Flask(__name__)
        self.COORDS = [
            (5, 10), (110, 10), (215, 10),
            (5, 65), (110, 65), (215, 65),
            (5, 120), (110, 120), (215, 120),
            (5, 180), (110, 180), (215, 180)
        ]
        self.setup_routes()

    def setup_routes(self):
        @self.app.route('/')
        def index():
            return 'Welcome to the Flask Server!'

        @self.app.route('/random-coords')
        def random_coords():
        # Check query parameter for truthy val
            use_toggle = request.args.get('use_toggle', 'false').lower() in ['true', '1', 't', 'y', 'yes']
            return self.pick_random_coords_and_replace(use_toggle)
 # IN_PROGRESS
        # TODO: Finish test case and ensure proper print statements
        @self.app.route('/multiple-random-buttons')
        def multiple_random_buttons():
            count = request.args.get('count', 1, type=int)
            use_toggle = request.args.get('use_toggle', 'false').lower() in ['true', '1', 't', 'y', 'yes']
            results = []
            used_coords = []
            for _ in range(count):
                try:
                    result, coords = self.pick_unique_random_coords(used_coords, use_toggle)
                    results.append(result)
                    used_coords.append(coords)
                except ValueError:
                    break  # Stop if we run out of unique coordinates
            return jsonify(results)




    def pick_random_coords_and_replace(self, use_toggle=False):
        randomCoords = random.choice(self.COORDS)


        textType = ("PUSH", "COCK")
        text = floor(random.random() * 100) == 69


        buttonType = "Toggle" if use_toggle else "PushButton"
        colors = f"0x780F,0xC618,{textType[text]}," if not use_toggle else "0xF800"
        formatted_string = f"{buttonType},{randomCoords[0]},{randomCoords[1]},{colors}"
        return formatted_string

    def pick_unique_random_coords(self, used_coords, use_toggle):
            available_coords = [coord for coord in self.COORDS if coord not in used_coords]
            if not available_coords:
                raise ValueError("No more unique coordinates available.")
            random_coords = random.choice(available_coords)
            button_type = "Toggle" if use_toggle else "PushButton"
            colors = "0x780F,0xC618,HERE," if not use_toggle else ""
            formatted_string = f"{button_type},{random_coords[0]},{random_coords[1]},{colors}"
            return formatted_string, random_coords


    def run(self):
        self.app.run(debug=True,host="10.152.43.98")

if __name__ == '__main__':
    flask_app = FlaskApp()
    flask_app.run()
