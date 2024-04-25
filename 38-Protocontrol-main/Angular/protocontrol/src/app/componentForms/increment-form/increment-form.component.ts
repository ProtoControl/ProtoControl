import { Component, Input } from '@angular/core';
import { IncrementImpl } from '../../models/increment.interface';
import { ConfigPanelService } from '../../services/config-panel.service';
import { COLORS } from '../../models/colors';

@Component({
  selector: 'app-increment-form',
  templateUrl: './increment-form.component.html',
  styleUrls: ['./increment-form.component.css']
})
export class IncrementFormComponent {

  @Input() increment: IncrementImpl;
  colors = COLORS;
  constructor(private sidePanelService: ConfigPanelService){
    this.increment = new IncrementImpl();
  }
  
  save(){
    console.log("Increment Saved");
    console.log(this.increment);
    this.sidePanelService.closePanel();
  }

  delete(){
    console.log("Increment Deleted");
    this.sidePanelService.closePanel();
  }
}
