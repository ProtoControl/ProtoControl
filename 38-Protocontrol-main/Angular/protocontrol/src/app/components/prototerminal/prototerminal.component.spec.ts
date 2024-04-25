import { ComponentFixture, TestBed } from '@angular/core/testing';

import { PrototerminalComponent } from './prototerminal.component';

describe('PrototerminalComponent', () => {
  let component: PrototerminalComponent;
  let fixture: ComponentFixture<PrototerminalComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [PrototerminalComponent]
    });
    fixture = TestBed.createComponent(PrototerminalComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
