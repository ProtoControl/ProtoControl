import { Component, Input } from '@angular/core';
import { ButtonImpl } from '../../models/button.interface';
import { ConfigPanelService } from '../../services/config-panel.service';
import { COLORS } from '../../models/colors';
@Component({
  selector: 'app-button-form',
  templateUrl: './button-form.component.html',
  styleUrls: ['./button-form.component.css']
})
export class ButtonFormComponent {

  @Input() button: ButtonImpl;
  colors = COLORS;
  constructor(private sidePanelService: ConfigPanelService) {
    this.button = new ButtonImpl();
  }

  save(){
    console.log("Button Saved");
    console.log(this.button);
    this.sidePanelService.closePanel();

  }
  delete(){
    console.log("Button Deleted");
    this.sidePanelService.closePanel();
  }
}
