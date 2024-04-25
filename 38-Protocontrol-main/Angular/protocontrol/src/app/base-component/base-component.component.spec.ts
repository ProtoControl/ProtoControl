import { ComponentFixture, TestBed } from '@angular/core/testing';

import { BaseComponentComponent } from './base-component.component';

describe('BaseComponentComponent', () => {
  let component: BaseComponentComponent;
  let fixture: ComponentFixture<BaseComponentComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [BaseComponentComponent]
    });
    fixture = TestBed.createComponent(BaseComponentComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
