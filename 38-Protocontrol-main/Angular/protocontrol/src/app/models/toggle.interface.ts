export interface BaseComponent {
    min?: number;
    max?: number;
    value?: number | boolean;
    color?: number;
    x: number;
    y: number;
    textColor?: number;
    text?: string;
}

export class ToggleImpl implements BaseComponent {
    min?: number;
    max?: number;
    value?:  number | boolean;
    color?: number;
    x: number;
    y: number;
    textColor?: number;
    text?: string;
    constructor(){
        this.x = 20;
        this.y = 100;
        this.color = 0xF800;
        this.value = false;
    }
}