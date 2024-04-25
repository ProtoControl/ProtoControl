import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProtosliderComponent } from './protoslider.component';

describe('ProtosliderComponent', () => {
  let component: ProtosliderComponent;
  let fixture: ComponentFixture<ProtosliderComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [ProtosliderComponent]
    });
    fixture = TestBed.createComponent(ProtosliderComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
