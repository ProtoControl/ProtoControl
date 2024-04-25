import requests

URL="https://random-word-api.herokuapp.com/word?number=3&length=5"


def test_random_word_api():
    response = requests.get(URL)
    
    print(response.text)



if __name__ == '__main__':
    test_random_word_api()