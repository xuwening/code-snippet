import React, {Component} from 'react';
import { createStore, applyMiddleware, combineReducers } from 'redux';
import { Provider } from 'react-redux';
import thunk from 'redux-thunk';


import * as reducers from '../reducers';
import CounterApp from './counterApp';

const createStoreWithMiddleware = applyMiddleware(thunk)(createStore);
const reducer = combineReducers(reducers);
const store = createStoreWithMiddleware(reducer);


export default class App extends Component {
  render() {

    a = (s) => s + 'a'
      b = (s) => s + 'b'
      c = (s) => s + 'c'

      l = [a,b,c]
      f = l.reduce((a,b) => (...args) => a(b(...args)))
      console.log(f('111'))
      
      l.reduce(function (a, b) {
          return function (...args) {
              return a(b(...args))
          }
      })

    return (
      <Provider store={store}>
        <CounterApp />
      </Provider>
    );
  }
}