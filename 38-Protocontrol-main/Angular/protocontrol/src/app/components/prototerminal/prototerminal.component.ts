import { Component } from '@angular/core';
import { TerminalImpl } from 'src/app/models/terminal.interface';
import { ConfigPanelService } from 'src/app/services/config-panel.service';

@Component({
  selector: 'app-prototerminal',
  templateUrl: './prototerminal.component.html',
  styleUrls: ['./prototerminal.component.css']
})
export class PrototerminalComponent {
  commandLines: string[] = [];
  currentCommand: string = '';
  cursorBlink: boolean = true;
  data: TerminalImpl = new TerminalImpl();

  constructor(private configPanelService: ConfigPanelService) { }

  ngOnInit(): void {
    setInterval(() => {
      this.cursorBlink = !this.cursorBlink;
    }, 500);
  }
  moveArrow(){
    console.log("Test Move Arrow");
  }
  openSidePanel(){
    const content = this.data;
    this.configPanelService.openPanel(content);
  }

  // Method to handle command submission
  submitCommand() {
    this.commandLines.push(this.currentCommand); // Add current command to command lines
    // You can implement command execution logic here
    this.currentCommand = ''; // Clear current command input
  }
}
