import { Component } from '@angular/core';


@Component({
  selector: 'menu',
  templateUrl: '../view/menu.component.html',
  styleUrls: ['../view/css/menu.component.css']
})
export class MenuComponent{
    menuItems : menuItemToAdd[];
    menuItemToAdd : menuItemToAdd;
    constructor(){

        console.log("Menu Succesfully Loaded!");

        this.menuItems = [];

        this.menuItemToAdd = {

            title: 'Database Data Overview',

            view : 'overview'

        }

        this.menuItems.push(this.menuItemToAdd);

        this.menuItemToAdd = {

            title: 'Database Test',

            view : 'test'

        }

        this.menuItems.push(this.menuItemToAdd);

       

    }
}

interface menuItemToAdd {
    
    title: string;

    view : string;

}