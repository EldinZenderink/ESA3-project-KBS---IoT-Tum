import { Component } from '@angular/core';
import {FormBuilder, FormGroup, Validators } from '@angular/forms';
import { DatabaseService } from '../services/database.service';

@Component({
  selector: 'test',
  templateUrl: '../view/test.component.html',
  styleUrls: ['../view/css/test.component.css']
})
export class TestComponent {
  SanitairForm: FormGroup;
  SanitairDBName:string = '';
  SanitairID:string = '';
  BuildingID:string = '';
  FloorID:string = '';  
  SensorForm: FormGroup;
  SensorID:string = '';
  SensorError:string = '';
  SensorPowerLevel:number = 0;
  SensorCount:number = 0;
  SensorMode:string = '';
  generatedJsonString: string;   
  generatedJson: any;
  selectDatabase : string;
  selectCollection: string;
  collectionToUploadTo : string;
  collections: any;
  databases: any;

  constructor(private fb: FormBuilder, private databaseService : DatabaseService){
    this.SanitairForm = fb.group({
      'SanitairID' : ['Unique Sanitair ID', Validators.required],
      'BuildingID' : ['Unique Building ID', Validators.required],
      'FloorID' : ['Unique Floor ID', Validators.required]
    })

    this.SensorForm = fb.group({
      'SensorID' : ['Unique Sensor ID', Validators.required],
      'SensorError' : ['0', Validators.required],
      'SensorPowerLevel' : ['100', Validators.required],
      'SensorCount' : [0, Validators.required],
      'SensorMode' : ['0', Validators.required]

    })

  }


  async generateSanitairPost(post){
    this.SanitairID = post.SanitairID;
    this.BuildingID = post.BuildingID;
    this.FloorID = post.FloorID;
    
      var temp = {"SanitairID" : this.SanitairID, "BuildingID" : this.BuildingID, "FloorID" : this.FloorID, "Sensors" : {}};
    var jsonObject = temp;
    this.generatedJson = jsonObject;
    this.generatedJsonString = JSON.stringify( this.generatedJson, undefined, '\t');
  }

  async generateSensorPost(post){
    this.SensorID = post.SensorID;
    this.SensorError = post.SensorError;
    this.SensorPowerLevel = post.SensorPowerLevel;
    this.SensorCount = post.SensorCount;
    this.SensorMode = post.SensorMode;
    var datetime = await this.databaseService.getDateTime();
    var date = datetime.localDate.split(' ')[0];
    var dateandtime = datetime.localDate;
    var jsonObject = {"id" : this.SensorID, "er" :this.SensorError, "pw" : this.SensorPowerLevel, "us" : Number(this.SensorCount), "md" : this.SensorMode, "latestupdate" : dateandtime};
    this.generatedJson.Sensors[this.SensorID] = jsonObject; 
    this.generatedJsonString = JSON.stringify( this.generatedJson, undefined, '\t');
  }


  async upsertToDocument(){      
      var datetime = await this.databaseService.getDateTime();
      var date = datetime.localDate.split(' ')[0] + "." + this.SanitairID + "_" + this.BuildingID + "_" +  this.FloorID;
      var temp = {};
      temp[date] = this.generatedJson;
      var response = this.databaseService.upsertFieldInDocument("tum", "sanitair", "FullDB", JSON.stringify(temp));
  }
}
