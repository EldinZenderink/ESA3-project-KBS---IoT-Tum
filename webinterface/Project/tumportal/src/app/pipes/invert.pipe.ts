import { PipeTransform, Pipe } from '@angular/core';

@Pipe({  name: 'invert'})
export class InvertPipe implements PipeTransform  {
  transform (values) {
    if (values) {
      return values.reverse();
    }
  }
}