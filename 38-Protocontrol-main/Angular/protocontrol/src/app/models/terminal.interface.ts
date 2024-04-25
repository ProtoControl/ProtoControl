export interface BaseComponent {
    min?: number;
    max?: number;
    value?:  number | boolean;
    color?: number;
    x: number;
    y: number;
    textColor?: number;
    text?: string;
}

export class TerminalImpl implements BaseComponent {
    x: number;
    y: number;
    min?: number;
    max?: number;
    value?:  number | boolean;
    text?: string;
    textColor?: number;
    color?: number;
    constructor(){
        this.color = 0xFFFF;
        this.x = 200;
        this.y = 100;
        this.min = 0;
        this.max = 20;
        this.value = this.max + this.min / 2;
    }
}