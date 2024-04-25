import { Component } from '@angular/core';
import { ConfigPanelService } from '../../services/config-panel.service';
import { SliderImpl } from '../../models/slider.interface';
@Component({
  selector: 'app-protoslider',
  templateUrl: './protoslider.component.html',
  styleUrls: ['./protoslider.component.css']
})
export class ProtosliderComponent {
  
  data: SliderImpl = new SliderImpl();
  constructor(private configPanelService: ConfigPanelService) { }

  moveArrow(){
    console.log("Test Move Arrow");
  }
  openSidePanel(){
    const content = this.data;
    this.configPanelService.openPanel(content)
  }
}
