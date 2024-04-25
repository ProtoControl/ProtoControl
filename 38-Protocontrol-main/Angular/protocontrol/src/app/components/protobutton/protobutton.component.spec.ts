import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProtobuttonComponent } from './protobutton.component';

describe('ProtobuttonComponent', () => {
  let component: ProtobuttonComponent;
  let fixture: ComponentFixture<ProtobuttonComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [ProtobuttonComponent]
    });
    fixture = TestBed.createComponent(ProtobuttonComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
