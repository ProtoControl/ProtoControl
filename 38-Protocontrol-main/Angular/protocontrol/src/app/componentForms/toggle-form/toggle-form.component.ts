import { Component, Input } from '@angular/core';
import { ToggleImpl } from '../../models/toggle.interface';
import { ConfigPanelService } from '../../services/config-panel.service';
import { COLORS } from '../../models/colors';

@Component({
  selector: 'app-toggle-form',
  templateUrl: './toggle-form.component.html',
  styleUrls: ['./toggle-form.component.css']
})
export class ToggleFormComponent {
  @Input() toggle: ToggleImpl;
  colors = COLORS;
  constructor(private sidePanelService: ConfigPanelService) {
    this.toggle = new ToggleImpl();
  }

  save(){
    console.log("Toggle Saved");
    console.log(this.toggle);
    this.sidePanelService.closePanel();
  }
  delete(){
    console.log("Toggle Deleted");
    this.sidePanelService.closePanel();
  }
}
