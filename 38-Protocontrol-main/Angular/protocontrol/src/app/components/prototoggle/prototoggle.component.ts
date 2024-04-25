import { Component } from '@angular/core';
import { ToggleImpl } from 'src/app/models/toggle.interface';
import { ConfigPanelService } from '../../services/config-panel.service';

@Component({
  selector: 'app-prototoggle',
  templateUrl: './prototoggle.component.html',
  styleUrls: ['./prototoggle.component.css']
})
export class PrototoggleComponent {
  data: ToggleImpl = new ToggleImpl();
  constructor(private configPanelService: ConfigPanelService) { }
  moveArrow(){
    console.log("Test Move Arrow")
  }
  openSidePanel(){
    console.log("Testing side panel")
    const content = this.data;
    this.configPanelService.openPanel(content)
  }
}
