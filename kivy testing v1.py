from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.togglebutton import ToggleButton
from kivy.uix.slider import Slider
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.widget import Widget
from kivy.graphics import Rectangle, Color
from kivy.uix.textinput import TextInput


class PushButton(Button):
    def __init__(self, **kwargs):
        super(PushButton, self).__init__(**kwargs)
        self.pressed = False
        self.location = self.assign(self.x, self.y)

    def assign(self, x, y):
        letters = [['A', 'B', 'C'], ['D', 'E', 'F'], ['G', 'H', 'I'], ['J', 'K', 'L']]
        x = x // 100
        y = y // 60
        return letters[y][x]

    def on_press(self):
        self.pressed = not self.pressed
        self.background_color = (1, 0, 0, 1) if self.pressed else (0, 1, 0, 1)
        print(f"{self.location}{self.pressed}")


class ToggleButtonWidget(ToggleButton):
    def __init__(self, **kwargs):
        super(ToggleButtonWidget, self).__init__(**kwargs)
        self.location = self.assign(self.x, self.y)
        self.state = 'normal'

    def assign(self, x, y):
        letters = [['A', 'B', 'C'], ['D', 'E', 'F'], ['G', 'H', 'I'], ['J', 'K', 'L']]
        x = x // 100
        y = y // 60
        return letters[y][x]

    def on_state(self, widget, value):
        print(f"{self.location}{value}")


class IncrementButton(BoxLayout):
    def __init__(self, **kwargs):
        super(IncrementButton, self).__init__(**kwargs)
        self.orientation = 'vertical'
        self.value = 0
        self.plus_button = Button(text="+", size_hint=(1, 0.5), on_press=self.increment)
        self.minus_button = Button(text="-", size_hint=(1, 0.5), on_press=self.decrement)
        self.add_widget(self.plus_button)
        self.add_widget(self.minus_button)

    def increment(self, instance):
        self.value += 1
        print(f"Value: {self.value}")

    def decrement(self, instance):
        self.value -= 1
        print(f"Value: {self.value}")


class SliderWidget(Slider):
    def __init__(self, **kwargs):
        super(SliderWidget, self).__init__(**kwargs)
        self.min = 0
        self.max = 100
        self.orientation = 'horizontal'
        self.value_track = True
        self.value_track_color = [1, 0, 0, 1]
        self.bind(value=self.on_value_change)

    def on_value_change(self, instance, value):
        print(f"Slider Value: {value}")


class MyApp(App):
    def build(self):
        layout = BoxLayout(orientation='vertical', padding=10, spacing=10)

        push_button = PushButton(text="Push Button", size_hint=(1, 0.2))
        toggle_button = ToggleButtonWidget(text="Toggle Button", size_hint=(1, 0.2))
        increment_button = IncrementButton(size_hint=(1, 0.2))
        slider_widget = SliderWidget(size_hint=(1, 0.2))

        layout.add_widget(push_button)
        layout.add_widget(toggle_button)
        layout.add_widget(increment_button)
        layout.add_widget(slider_widget)

        return layout


if __name__ == '__main__':
    MyApp().run()
