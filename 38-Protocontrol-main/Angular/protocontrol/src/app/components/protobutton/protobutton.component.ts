import { Component } from '@angular/core';
import { ConfigPanelService } from '../../services/config-panel.service';
import { ButtonImpl } from '../../models/button.interface';
import { coords } from '../../models/coords';

@Component({
  selector: 'app-protobutton',
  templateUrl: './protobutton.component.html',
  styleUrls: ['./protobutton.component.css']
})

export class ProtobuttonComponent {
  data: ButtonImpl = new ButtonImpl();

  constructor(private configPanelService: ConfigPanelService) {

  }

  onClick(){
    console.log("Button Press!!");
  }
  moveArrow(){
    console.log("Test Move Arrow")
  }
  openSidePanel(){
    console.log("Testing side panel")
    const content = this.data;
    this.configPanelService.openPanel(content)
  }

}
