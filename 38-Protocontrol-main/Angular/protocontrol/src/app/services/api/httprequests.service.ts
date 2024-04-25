import { Injectable, OnDestroy } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, Subscription, Subject } from 'rxjs';
import { map, takeUntil } from 'rxjs/operators';
import { ConfigPanelService } from '../config-panel.service';
import { ButtonImpl } from '../../models/button.interface';
import { ProtobuttonComponent } from 'src/app/components/protobutton/protobutton.component';
import { ProtosliderComponent } from 'src/app/components/protoslider/protoslider.component';
import { PrototoggleComponent } from 'src/app/components/prototoggle/prototoggle.component';
import { ProtoincComponent } from 'src/app/components/protoinc/protoinc.component';
import { ToggleImpl } from '../../models/toggle.interface';
import { IncrementImpl } from '../../models/increment.interface';
import { SliderImpl } from '../../models/slider.interface';
import { TerminalImpl } from 'src/app/models/terminal.interface';
import { coords } from '../../models/coords';
import { PrototerminalComponent } from 'src/app/components/prototerminal/prototerminal.component';


@Injectable({
  providedIn: 'root'
})
export class HttprequestsService implements OnDestroy {
  private subscription: Subscription;
  private unsubscribe$: Subject<void> = new Subject<void>();
  coords = coords;
  private deviceID: string = "Default";

  constructor(private http: HttpClient, private configPanel: ConfigPanelService) {
    this.subscription = this.configPanel.generateButtonClick$
      .pipe(takeUntil(this.unsubscribe$))
      .subscribe(locations => {
        this.formatString(locations);
      });
  }

  ngOnDestroy(): void {
    this.unsubscribe$.next();
    this.unsubscribe$.complete();
    this.subscription.unsubscribe();
  }

  formatString(components: Map<number, (ButtonImpl | SliderImpl | ToggleImpl | IncrementImpl | TerminalImpl)>) {
    console.log(components);
    let outputString = "";
    components.forEach((component, index) => {
      if (component instanceof ProtobuttonComponent) {
        console.log(component);
        outputString += "PushButton,";
        outputString += this.coords[index][0] + "," + this.coords[index][1] + ",";
        outputString += (component.data.textColor !== undefined ? component.data.textColor : '') + ",";
        outputString += (component.data.color !== undefined ? component.data.color.toString(16) : '') + ",";
        outputString += (component.data.text !== undefined ? component.data.text.toString() : '') + ",";
      } else if (component instanceof ProtosliderComponent) {
        outputString += "Slider,";
        outputString += this.coords[index][0] + "," + this.coords[index][1] + ",";
        outputString += '0,'// Orientation
        outputString += component.data.min + ',';
        outputString += component.data.max + ',';
      } else if (component instanceof PrototoggleComponent) {
        outputString += "ToggleButton,";
        outputString += this.coords[index][0] + "," + this.coords[index][1] + ",";
        outputString += (component.data.textColor !== undefined ? component.data.textColor.toString(16) : '') + ",";
        outputString += (component.data.color !== undefined ? component.data.color.toString(16) : '') + ",";
        //outputString += (component.value !== undefined ? component.value.toString() : '') + ","; TODO: see if we want an inittal value
      } else if (component instanceof ProtoincComponent) {
        outputString += "Increment,";
        outputString += this.coords[index][0] + "," + this.coords[index][1] + ",";
        outputString += '0xA0A0,';
        //TODO: Color doesn't work for some reason
        //outputString += (component.data.color != undefined ? component.color.toString(16) : '') + ",";
      } else if (component instanceof PrototerminalComponent) {
        outputString += "Terminal,";
        outputString += this.coords[index][0] + "," + this.coords[index][1] + ",";
      }
      console.log(outputString);
      outputString += "|,";
    });
    let id = prompt("Please Enter A Device ID", "Default");
    if(id != null){
      this.deviceID = id;
    }
    this.createEntry([this.deviceID, outputString, String(this.configPanel.activeProfile)]).subscribe(result => {
      console.log(result);
    });
    /*
    this.getRandomWords(3, 5).subscribe(words => {
      const keyValue:string = words.join('');
      //console.log("KeyValue (id string): " + keyValue);
      alert("KeyValue: " + keyValue);
      this.createEntry([keyValue, outputString]).subscribe(result => {
        console.log(result);
      });
    }

  );
  */
  }
  getRandomWord(length: number = 5): Observable<string> {
    return this.http.get<string>(`https://random-word-api.herokuapp.com/word?length=${length}`);
  }
  getRandomWords(count: number = 3, length: number = 5): Observable<string[]> {
    return this.http.get<string[]>(`https://random-word-api.herokuapp.com/word?number=${count}&length=${length}`);
  }
  validateWords(words: string[]): Observable<boolean> {
    return this.http.post<boolean>('http://localhost:3000/validate', words);
  }
  createEntry(words: string[]): Observable<boolean> {
    return this.http.post<boolean>('https://protocontrol.pythonanywhere.com/new-entry', { "Device": words[0], "Layout": words[1], "Profile": words[2]});
  }
}
