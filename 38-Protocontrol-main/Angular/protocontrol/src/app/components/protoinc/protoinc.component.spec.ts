import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProtoincComponent } from './protoinc.component';

describe('ProtoincComponent', () => {
  let component: ProtoincComponent;
  let fixture: ComponentFixture<ProtoincComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [ProtoincComponent]
    });
    fixture = TestBed.createComponent(ProtoincComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
