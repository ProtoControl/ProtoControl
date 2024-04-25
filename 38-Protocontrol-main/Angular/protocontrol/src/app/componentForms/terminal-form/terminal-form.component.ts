import { Component, Input } from '@angular/core';
import { TerminalImpl } from 'src/app/models/terminal.interface';
import { ConfigPanelService } from 'src/app/services/config-panel.service';
import { COLORS } from 'src/app/models/colors';


@Component({
  selector: 'app-terminal-form',
  templateUrl: './terminal-form.component.html',
  styleUrls: ['./terminal-form.component.css']
})

export class TerminalFormComponent {
  colors = COLORS;
  @Input() terminal: TerminalImpl;

  constructor(private sidePanelService: ConfigPanelService) {
    this.terminal = new TerminalImpl();
  }
  save(){
    console.log("Terminal Saved");
    console.log(this.terminal);
    this.sidePanelService.closePanel();
  }
  delete(){
    console.log("Terminal Deleted");
    this.sidePanelService.closePanel();
  }

}
