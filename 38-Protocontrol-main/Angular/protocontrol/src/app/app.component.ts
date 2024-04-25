import { CdkDragDrop } from '@angular/cdk/drag-drop';
import { Component, ComponentFactoryResolver, ViewChild ,ViewChildren, ViewContainerRef, Type, ComponentRef, QueryList } from '@angular/core';
import { SidebarComponent } from './sidebar/sidebar.component';
import { ConfigPanelService } from './services/config-panel.service';
import { HttprequestsService } from './services/api/httprequests.service';
import { ProtosliderComponent } from './components/protoslider/protoslider.component';
import { Subscription } from 'rxjs';
import { ButtonImpl } from './models/button.interface';
import { SliderImpl } from './models/slider.interface';
import { ToggleImpl } from './models/toggle.interface';
import { IncrementImpl } from './models/increment.interface';
import { TerminalImpl } from './models/terminal.interface';
@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'protocontrol';
  gridCells = new Array(12).fill(null);
  gridIds: string[] = Array(12).fill(0).map((_, i) => 'container-' + i);
  componentData: any;
  componentInfo: Map<number, (ButtonImpl | SliderImpl | ToggleImpl | IncrementImpl | TerminalImpl)> = new Map();
  disabledIndices: number[] = [];
  @ViewChildren('dynamicComponentContainer', { read: ViewContainerRef }) containers!: QueryList<ViewContainerRef>;
  @ViewChild(SidebarComponent, { static: false }) sidebar?: SidebarComponent;

  constructor(private componentFactoryResolver: ComponentFactoryResolver, private configPanelService: ConfigPanelService, private httpReq : HttprequestsService ) {
  }
  onDrop(event: CdkDragDrop<any>, idx: number) {
    console.log(this.componentData);
    console.log("Index:", idx);
    console.log("Disabled Indices:", this.disabledIndices);
    if(this.disabledIndices.includes(idx)){
      alert("Please leave room for the Slider... Try another cell.");
      return;
    }
    if (event.previousContainer === event.container) {
      return;
    }
    if (!this.sidebar || !this.sidebar.currentlyDraggedComponent) return;
    const componentType: Type<any> = this.sidebar.currentlyDraggedComponent;
    console.log("Component Type:", componentType);
    const targetId = event.container.id;
    const index = +targetId.split('-')[1];
    this.configPanelService.fetchIndex(index);
    const viewContainerRef = this.containers.toArray()[index];
    this.componentData = this.loadComponent(viewContainerRef, componentType);

    this.configPanelService.dataMap.set(this.componentData._rootLView[0].id.split('-')[1], this.componentData.instance);

    this.componentInfo.set(parseInt(this.componentData._rootLView[0].id.split('-')[1]), this.componentData.instance);

    if(this.componentData.instance instanceof ProtosliderComponent){
      this.sliderDisable(this.componentData._rootLView[0].id.split('-')[1]);
      console.log("\n\n\n Testing \n\n\n");
    }
    console.log(this.componentInfo);
  }

  handlePanelStateChange(state: boolean) {
    console.log("Side panel state changed:", state);
  }
  loadComponent(container: ViewContainerRef, componentType: Type<any>): ComponentRef<any> {
    const componentFactory = this.componentFactoryResolver.resolveComponentFactory(componentType);
    console.log("Component Factory:", container);
    return container.createComponent(componentFactory);
  }

  sliderDisable(index: number){
      const numCols = 3;
      const row = Math.floor(index / numCols);
      const sameRowIndexes: number[] = [];
      for (let i = 0; i < numCols; i++) {
          const idx = row * numCols + i;
          sameRowIndexes.push(idx);
      }
      console.log(sameRowIndexes);
      this.disabledIndices.push(...sameRowIndexes);
      return sameRowIndexes;
  }
  indexToLetter(index: number): string {
    return String.fromCharCode(65 + index);
  }

  ngOnInit() {
    this.configPanelService.panelIndex.subscribe(state => {
      //this.isOpen = state;
      //console.log("Testing panel state");
      //this.sidePanelService.panelStateSource(state);
    })

    this.configPanelService.panelContent.subscribe(content => {
      //this.settings = content;
      //console.log("Hello");
      //console.log(this.settings)
    })
  }
}
