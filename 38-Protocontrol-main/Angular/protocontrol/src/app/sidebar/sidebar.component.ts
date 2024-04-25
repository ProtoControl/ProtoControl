  import { Component, Output, Input, EventEmitter, Type } from '@angular/core';
  import { CdkDragDrop, moveItemInArray } from '@angular/cdk/drag-drop';
  import { ProtobuttonComponent } from '../components/protobutton/protobutton.component';
  import { ProtosliderComponent } from '../components/protoslider/protoslider.component';
  import { PrototoggleComponent } from '../components/prototoggle/prototoggle.component';
  import { ProtoincComponent } from '../components/protoinc/protoinc.component';
import { ConfigPanelService } from '../services/config-panel.service';
import { PrototerminalComponent } from '../components/prototerminal/prototerminal.component';

  @Component({
    selector: 'app-sidebar',
    templateUrl: './sidebar.component.html',
    styleUrls: ['./sidebar.component.css']
  })
  export class SidebarComponent {
    colSize = 1;
    profileNumber: number = 1;
    @Input() connectedDropLists: string[] = [];
    @Output() componentSelected = new EventEmitter<any>();
    currentlyDraggedComponent: Type<Component> | null = null;

    items = [
      { component: ProtobuttonComponent, label: 'Button' },
      { component: ProtosliderComponent, label: 'Slider' },
      { component: PrototoggleComponent, label: 'Toggle' },
      { component: ProtoincComponent, label: 'Increment/Decrement' },
      { component: PrototerminalComponent, label: 'Terminal'}
    ];

    constructor(private sidePanelService: ConfigPanelService){}

    onDragStart(item: any) {
      console.log("Drag Start");
      console.log(item);
      if(item.component.label == "Slider"){
        this.colSize = 3;
      }
      this.currentlyDraggedComponent = item.component;
    }

    selectComponent(componentType: any) {
      this.componentSelected.emit(componentType);
    }
    drop(event: CdkDragDrop<any[]>) {
      console.log(event.container);
      //console.log(this.currentlyDraggedComponent);
      moveItemInArray(this.items, event.previousIndex, event.currentIndex);
      }



  onSelectionChange(){
    this.sidePanelService.setProfile(this.profileNumber);
  }
  generateString(){
    this.sidePanelService.triggerStringGeneration();
    }
  }
