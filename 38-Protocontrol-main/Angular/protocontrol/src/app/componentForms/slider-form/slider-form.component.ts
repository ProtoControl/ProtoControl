import { Component, Input } from '@angular/core';
import { SliderImpl } from 'src/app/models/slider.interface';
import { ConfigPanelService } from '../../services/config-panel.service';
import { COLORS } from 'src/app/models/colors';

@Component({
  selector: 'app-slider-form',
  templateUrl: './slider-form.component.html',
  styleUrls: ['./slider-form.component.css']
})
export class SliderFormComponent {
  colors = COLORS;
  @Input() slider: SliderImpl;

  constructor(private sidePanelService: ConfigPanelService) {
    this.slider = new SliderImpl();
  }

  save(){
    console.log("Slider Saved");
    console.log(this.slider);
    this.sidePanelService.closePanel();
  }
  delete(){
    console.log("Slider Deleted");
    this.sidePanelService.closePanel();
  }
}
