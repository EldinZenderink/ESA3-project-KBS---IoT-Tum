import { Component } from '@angular/core';
import { DatabaseService } from '../services/database.service';
declare var $: any;

@Component({
  selector: 'overview',
  templateUrl: '../view/overview.component.html',
  styleUrls: ['../view/css/overview.component.css']
})
export class OverviewComponent {
    fulldb : any;
    daycount: number;
    constructor( private databaseService : DatabaseService){
        this.runDbCmd()
        this.daycount = 1;
    }

    /*
        function gets the full database from mlab, then parses it so that it counts the total uses of toilets and gets the latest update time.
        runs once a second
    */
    async runDbCmd(){

        setInterval(async()=>{
            var db = await this.databaseService.getDocument("tum", "sanitair", "FullDB");
            var counttoiletuses = 0;
            var latestUpdate = 0;
            var totalsanitairusages = 0;
            for(var date in db){
                if(date != "_id"){                
                    for(var sanitair in db[date]){
                        counttoiletuses = 0;
                        latestUpdate = 0;
                        for(let sensor in db[date][sanitair].Sensors){
                            var localtime = new Date();
                            if(db[date][sanitair].Sensors[sensor].md == "1"){
                                counttoiletuses += db[date][sanitair].Sensors[sensor].us;
                                var time = db[date][sanitair].Sensors[sensor].latestupdate.split(' ')[1] + db[date][sanitair].Sensors[sensor].latestupdate.split(' ')[2] ;
                                
                                if(time.indexOf(":")!= -1){
                                    if(time.indexOf("PM") > 0){
                                        localtime.setHours((parseInt(time.substring(0,2)) + 12));
                                        localtime.setMinutes(time.substring(3, 5));
                                        localtime.setSeconds(time.substring(6, 8));
                                        
                                    } else {
                                        localtime.setHours(time.substring(0,2));
                                        localtime.setMinutes(time.substring(3, 5));
                                        localtime.setSeconds(time.substring(6, 8));
                                        
                                    }    
                                } else {
                                    if(time.indexOf("PM") > 0){
                                        localtime.setHours((parseInt(time.substring(0,2)) + 12));
                                        localtime.setMinutes(time.substring(2,4));
                                        localtime.setSeconds(time.substring(4, 6));
                                        
                                    } else {
                                        localtime.setHours(time.substring(0,2));
                                        localtime.setMinutes(time.substring(2, 4));
                                        localtime.setSeconds(time.substring(4, 6));
                                        
                                    }    
                                }
                                
                                                    
                            } else {
                                totalsanitairusages = Math.round( db[date][sanitair].Sensors[sensor].us / 2);
                                var time = db[date][sanitair].Sensors[sensor].latestupdate.split(' ')[1] + db[date][sanitair].Sensors[sensor].latestupdate.split(' ')[2] ;
                                console.log(time);
                                 if(time.indexOf(":") != -1){
                                    if(time.indexOf("PM") > 0){
                                        localtime.setHours((parseInt(time.substring(0,2)) + 12));
                                        localtime.setMinutes(time.substring(3, 5));
                                        localtime.setSeconds(time.substring(6, 8));
                                        
                                    } else {
                                        localtime.setHours(time.substring(0,2));
                                        localtime.setMinutes(time.substring(3, 5));
                                        localtime.setSeconds(time.substring(6, 8));
                                        
                                    }    
                                } else {
                                    if(time.indexOf("PM") > 0){
                                        localtime.setHours((parseInt(time.substring(0,2)) + 12));
                                        localtime.setMinutes(time.substring(2,4));
                                        localtime.setSeconds(time.substring(4, 6));
                                        
                                    } else {
                                        localtime.setHours(time.substring(0,2));
                                        localtime.setMinutes(time.substring(2, 4));
                                        localtime.setSeconds(time.substring(4, 6));
                                        
                                    }    
                                }
                            }

                            
                            if(localtime.getTime() > latestUpdate){
                                latestUpdate = localtime.getTime(); 
                            }  
                        }
                        var updateDate = new Date(latestUpdate);
                        var timestr = updateDate.toTimeString();
                        db[date][sanitair]["LatestUpdate"] = timestr.split(' ')[0];
                        db[date][sanitair]["TotalToiletUses"] = counttoiletuses;
                        db[date][sanitair]["TotalSanitairUses"] = totalsanitairusages;
                    }
                }
                
            }

            this.fulldb = db;
            
        }, 1000);
        
        
    }

    setDayCount(days:number){
        this.daycount = days;
    }
}
