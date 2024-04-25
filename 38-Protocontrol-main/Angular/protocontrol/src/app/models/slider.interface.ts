export interface BaseComponent {
    text?: string;
    min?: number;
    max?: number;
    value?:  number | boolean; // Initial value, if left undefined, defaults to (max + min) / 2
    x: number;
    y: number;
    color?: number;
    textColor?: number;
};

export class SliderImpl implements BaseComponent{
    min?: number;
    max?: number;
    value?:  number | boolean; // Initial value, if left undefined, defaults to (max + min) / 2
    x: number;
    y: number;
    color?: number;
    textColor?: number;
    text?: string;
    constructor() {
        this.text = "";
        this.min = 0;
        this.max = 100;
        this.x = 0;
        this.y = 0;
        this.value = this.max + this.min / 2;
    }
}