import { Component } from '@angular/core';
import { CdkDragDrop, moveItemInArray, transferArrayItem } from '@angular/cdk/drag-drop';
import { SidePanelComponent } from '../side-panel/side-panel.component';

interface GridItem {
  id: number;
  size: { rows: number; cols: number };
}

@Component({
  selector: 'app-canvas',
  templateUrl: './canvas.component.html',
  styleUrls: ['./canvas.component.css']
})
export class CanvasComponent {
  gridItems: GridItem[] = Array.from({ length: 12 }, (_, index) => ({
    id: index,
    size: { rows: 1, cols: 1 }
  }));

  drop(event: CdkDragDrop<string[]>) {
    console.log(event.container);
    console.log(SidePanelComponent.name);
    console.log("Testing Canvas Drop");
    if (event.previousContainer !== event.container) {
      transferArrayItem(event.previousContainer.data,
                        event.container.data,
                        event.previousIndex,
                        event.currentIndex);
    }
    else {

      moveItemInArray(event.container.data, event.previousIndex, event.currentIndex);
    }
  }
}