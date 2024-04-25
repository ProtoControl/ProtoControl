import { Injectable } from '@angular/core';
import { BehaviorSubject, Subject } from 'rxjs';
import { ButtonImpl } from '../models/button.interface';
import { ToggleImpl } from '../models/toggle.interface';
import { IncrementImpl } from '../models/increment.interface';
import { SliderImpl } from '../models/slider.interface';
import { TerminalImpl } from '../models/terminal.interface';


@Injectable({
  providedIn: 'root'
})
export class ConfigPanelService {
  orderOfIndexes: Array<number> = new Array();
  activeProfile: number = 1;
  orderOfComponents: Array<(ButtonImpl | SliderImpl | ToggleImpl | IncrementImpl | TerminalImpl)> = new Array(0);
  componentInfo: Map<number, (ButtonImpl | SliderImpl | ToggleImpl | IncrementImpl | TerminalImpl)> = new Map();
  dataMap: Map<number, (ButtonImpl | SliderImpl | ToggleImpl | IncrementImpl | TerminalImpl)>;
  constructor(){
    this.dataMap = new Map<number, (ButtonImpl | SliderImpl | ToggleImpl | IncrementImpl | TerminalImpl)>;
  }
  public panelContentSource = new BehaviorSubject<ButtonImpl | IncrementImpl | ToggleImpl | SliderImpl | TerminalImpl | null>(null);
  panelContent = this.panelContentSource.asObservable();

  public panelSourceIndex = new BehaviorSubject<number | null>(null);
  panelIndex = this.panelSourceIndex.asObservable();

  private prepareString = new Subject<Map<number, (ButtonImpl | SliderImpl | ToggleImpl | IncrementImpl | TerminalImpl)>>();
  generateButtonClick$ = this.prepareString.asObservable();

  triggerStringGeneration(){
    console.log("Triggered");

    console.log("Preparing String \n\n\n");
    console.log(this.dataMap);
    this.prepareString.next(this.dataMap);
  }

  setProfile(profile: number){
    this.activeProfile = profile;
  }

  updateMap(index: number, component: (ButtonImpl | SliderImpl | ToggleImpl | IncrementImpl | TerminalImpl)){
    this.dataMap.set(index, component);
  }
  fetchIndex(idx: number){
    this.panelSourceIndex.next(idx);
    if(this.orderOfIndexes.includes(idx)){
      console.error("Element already placed at this index");
    }
    else{
      this.orderOfIndexes.push(idx);
    }
  }


  openPanel(content: any) {
    this.panelSourceIndex.next(content); // toggle isOpen state of side-panel comp
    this.panelContentSource.next(content); // update settings property of side-panel comp
    console.log(this.panelContentSource);
    console.log(this.panelSourceIndex);
    //this.dataMap
    //this.orderOfComponents.push(content); // record the component dropped on the grid
  }

  closePanel() {
    this.panelContentSource.next(null);
  }
}
