import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ToggleFormComponent } from './toggle-form.component';

describe('ToggleFormComponent', () => {
  let component: ToggleFormComponent;
  let fixture: ComponentFixture<ToggleFormComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [ToggleFormComponent]
    });
    fixture = TestBed.createComponent(ToggleFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
