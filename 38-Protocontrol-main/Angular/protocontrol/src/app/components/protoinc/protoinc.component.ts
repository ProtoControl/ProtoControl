import { Component } from '@angular/core';
import { ConfigPanelService } from '../../services/config-panel.service';
import { IncrementImpl } from '../../models/increment.interface';

@Component({
  selector: 'app-protoinc',
  templateUrl: './protoinc.component.html',
  styleUrls: ['./protoinc.component.css']
})
export class ProtoincComponent {
  data = new IncrementImpl();
  private value: number = 0;
  constructor(private configPanelService: ConfigPanelService) {  }

  onInc(){
    this.value++;
    console.log(this.value);
  }

  onDec(){
    this.value--;
    console.log(this.value);
  }

  moveArrow(){
    console.log("Test Move Arrow")
  }
  openSettings(){
    console.log("Test Settings Icon")
  }
  openSidePanel(){
    console.log("Testing side panel")
    const content = this.data;
    this.configPanelService.openPanel(content)
  }

}
