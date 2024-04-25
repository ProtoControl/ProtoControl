import { ComponentFixture, TestBed } from '@angular/core/testing';

import { PrototoggleComponent } from './prototoggle.component';

describe('PrototoggleComponent', () => {
  let component: PrototoggleComponent;
  let fixture: ComponentFixture<PrototoggleComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [PrototoggleComponent]
    });
    fixture = TestBed.createComponent(PrototoggleComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
