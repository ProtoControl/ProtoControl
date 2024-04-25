import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { DragDropModule } from '@angular/cdk/drag-drop';
import { FormsModule } from '@angular/forms';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { SidebarComponent } from './sidebar/sidebar.component';
import { CanvasComponent } from './canvas/canvas.component';
import { ProtobuttonComponent } from './components/protobutton/protobutton.component';
import { ProtosliderComponent } from './components/protoslider/protoslider.component';
import { PrototoggleComponent } from './components/prototoggle/prototoggle.component';
import { ProtoincComponent } from './components/protoinc/protoinc.component';
import { ImageUploadComponent } from './image-upload/image-upload.component';
import { SidePanelComponent } from './side-panel/side-panel.component';
import { ButtonFormComponent } from './componentForms/button-form/button-form.component';
import { SliderFormComponent } from './componentForms/slider-form/slider-form.component';
import { ToggleFormComponent } from './componentForms/toggle-form/toggle-form.component';
import { IncrementFormComponent } from './componentForms/increment-form/increment-form.component';
import { PrototerminalComponent } from './components/prototerminal/prototerminal.component';
import { TerminalFormComponent } from './componentForms/terminal-form/terminal-form.component';

@NgModule({
  declarations: [
    AppComponent,
    SidebarComponent,
    CanvasComponent,
    ProtobuttonComponent,
    ProtosliderComponent,
    PrototoggleComponent,
    ProtoincComponent,
    ImageUploadComponent,
    SidePanelComponent,
    ButtonFormComponent,
    SliderFormComponent,
    ToggleFormComponent,
    IncrementFormComponent,
    PrototerminalComponent,
    TerminalFormComponent
  ],
  imports: [
    DragDropModule,
    FormsModule,
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
