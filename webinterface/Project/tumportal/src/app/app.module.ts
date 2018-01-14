import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HttpModule } from '@angular/http';
import {FormsModule, ReactiveFormsModule} from '@angular/forms';

import {NgbModule} from '@ng-bootstrap/ng-bootstrap';



import { AppComponent } from './app.component';
import {MenuComponent} from './components/menu.component';
import {OverviewComponent} from './components/overview.component';
import {TestComponent} from './components/test.component';

import{DatabaseService} from './services/database.service'

import{KeysPipe} from './pipes/jsonkey.pipe';
import{InvertPipe} from './pipes/invert.pipe';

const appRoutes: Routes = [
  {
    path: '',
    redirectTo: "/overview",
    pathMatch: 'full'
  },
   {  
    path: 'test',  
    component: TestComponent,  
    data: { title: 'MongoDB Test' }  
  }, {  
    path: 'overview',  
    component: OverviewComponent,  
    data: { title: 'MongoDB Overview' }  
  }
];
@NgModule({
  declarations: [
    AppComponent,
    MenuComponent,
    OverviewComponent,
    TestComponent,
    KeysPipe,
    InvertPipe
  ],
  imports: [
    BrowserModule,
    FormsModule,
    ReactiveFormsModule,
    HttpModule,
    RouterModule.forRoot(appRoutes, { enableTracing: true }), 
    NgbModule.forRoot()
  ],
  providers: [DatabaseService],
  bootstrap: [AppComponent, MenuComponent, OverviewComponent,TestComponent]
})
export class AppModule { }
