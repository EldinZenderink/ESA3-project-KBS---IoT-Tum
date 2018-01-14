import {Injectable} from '@angular/core';
import {Http, Headers, RequestOptions} from '@angular/http';
import {Observable} from 'rxjs/Rx';
import {Subject} from 'rxjs/Rx';
import {BehaviorSubject} from 'rxjs/Rx';

import 'rxjs/add/observable/of'; //proper way to import the 'of' operator
import 'rxjs/add/operator/share';
import 'rxjs/add/operator/map';

@Injectable()
export class DatabaseService {

    key : String;

    constructor(private http: Http){
        console.log("Initiated database connection!");
        this.key = "zV5Ff3TEUTaSDvUoWVu3Y1q_1YkFZQNL";
    }

    async getDatabases(){
        const response = await this.http.get( encodeURI('https://api.mlab.com/api/1/databases?apiKey=' + this.key)).toPromise();
        
        return response.json();
        
    }

    async getCollections(database:String){
        
        const response = await this.http.get( encodeURI('https://api.mlab.com/api/1/databases/' + database + '/collections?apiKey=' + this.key)).toPromise();
        
        return response.json();

    }

    async getDocuments(database:String, collection:String){
        const response = await this.http.get( encodeURI('https://api.mlab.com/api/1/databases/' + database + '/collections/' + collection + '?apiKey=' + this.key)).toPromise();
        
        return response.json();
    }

    async getDocument(database:String, collection:String, id:String){
        const response = await this.http.get( encodeURI('https://api.mlab.com/api/1/databases/' + database + '/collections/' + collection + '/' + id + '?apiKey=' + this.key)).toPromise();

        return response.json();
    }


    async uploadDocument(database:String, collection:String, json:String){
        let body = json;
        let headers = new Headers({'Content-Type': 'application/json'});
        let options = new RequestOptions({headers: headers});

         const response = await this.http.post(encodeURI('https://api.mlab.com/api/1/databases/' + database + '/collections/' + collection + '?apiKey=' + this.key), body, options).toPromise();
        
        return response.json();

    }

    async upsertFieldInDocument(database:String, collection:String, id:String, json:String){
        let body = '{"$set" : ' + json + "}";
        let headers = new Headers({'Content-Type': 'application/json'});
        let options = new RequestOptions({headers: headers});

        const response = await this.http.put(encodeURI('https://api.mlab.com/api/1/databases/' + database + '/collections/' + collection + '/' + id + '?apiKey=' + this.key + '&u=true'), body, options).toPromise();
        
        return response.json();

    }

    async getDateTime(){
        
        let headers = new Headers({"X-Requested-With": "TUM"});
        let options = new RequestOptions({headers: headers});
        const response = await this.http.get( encodeURI('http://cors-anywhere.herokuapp.com/http://convert-unix-time.com/api?timestamp=now&timezone=amsterdam&format=english12')).toPromise();        
        return response.json();
    }

}