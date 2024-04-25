export interface BaseComponent {
    min?: number;
    max?: number ;
    value?: number | boolean;
    color?: number;
    x: number;
    y: number;
    textColor?: number;
    text?: string;
}

export class ButtonImpl implements BaseComponent {
    min?: number;
    max?: number;
    value?: number | boolean;
    color?: number;
    x: number;
    y: number;
    textColor?: number;
    text?: string;
    constructor() {
        this.x = 20;
        this.y = 20;
        this.color = 0xC618;
        this.textColor = 0x780F;
        this.text = 'Push';
      }
}