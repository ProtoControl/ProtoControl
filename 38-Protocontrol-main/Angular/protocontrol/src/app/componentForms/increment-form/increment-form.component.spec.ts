import { ComponentFixture, TestBed } from '@angular/core/testing';

import { IncrementFormComponent } from './increment-form.component';

describe('IncrementFormComponent', () => {
  let component: IncrementFormComponent;
  let fixture: ComponentFixture<IncrementFormComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [IncrementFormComponent]
    });
    fixture = TestBed.createComponent(IncrementFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
