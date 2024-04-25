import { Component, EventEmitter, OnInit, Output, Input } from '@angular/core';
import { ConfigPanelService } from '../services/config-panel.service';
import { ButtonImpl } from '../models/button.interface';
import { ToggleImpl } from '../models/toggle.interface';
import { IncrementImpl } from '../models/increment.interface';
import { SliderImpl } from '../models/slider.interface';
import { TerminalImpl } from '../models/terminal.interface';


@Component({
  selector: 'app-side-panel',
  templateUrl: './side-panel.component.html',
  styleUrls: ['./side-panel.component.css']
})
export class SidePanelComponent implements OnInit {
  @Output() panelStateChange = new EventEmitter<boolean>();
  @Input() settings: ButtonImpl | IncrementImpl | ToggleImpl | SliderImpl | TerminalImpl | null = null;
  isOpen: boolean;
  constructor(private sidePanelService: ConfigPanelService) {
    this.isOpen = true;
  }



  isButton(): boolean {
    return this.settings instanceof ButtonImpl;
  }
  isIncrement(): boolean {
    return this.settings instanceof IncrementImpl;
  }
  isSlider(): boolean {
    return this.settings instanceof SliderImpl;
  }
  isToggle(): boolean {
    return this.settings instanceof ToggleImpl;
  }
  isTerminal(): boolean {
    return this.settings instanceof TerminalImpl;
  }


  ngOnInit() {  //TODO: Shouldn't show both isOpen conditionals at the same time
    this.sidePanelService.panelIndex.subscribe(state => {
      this.isOpen = !this.isOpen;
      console.log("Opened by component dropped on: " + state);
      console.log(this.isOpen);
    })

    this.sidePanelService.panelContent.subscribe(content => {
      this.settings = content;

    })
  }
}