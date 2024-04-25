import { Component } from '@angular/core';

@Component({
  selector: 'app-image-upload',
  templateUrl: './image-upload.component.html',
  styleUrls: ['./image-upload.component.css']
})
export class ImageUploadComponent {

  onClick(): void{
    console.log("Tesestin");
    return;
  }
  onFileChange(event: any): void {
    const file = event.target.files[0];
    if (file) {
      // Handle the image file. You can upload it or process it as needed.
      console.log('File chosen:', file);
    }
  }
}
