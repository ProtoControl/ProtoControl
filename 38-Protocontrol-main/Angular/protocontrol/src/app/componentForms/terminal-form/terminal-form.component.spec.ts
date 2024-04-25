import { ComponentFixture, TestBed } from '@angular/core/testing';

import { TerminalFormComponent } from './terminal-form.component';

describe('TerminalFormComponent', () => {
  let component: TerminalFormComponent;
  let fixture: ComponentFixture<TerminalFormComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [TerminalFormComponent]
    });
    fixture = TestBed.createComponent(TerminalFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
