

import React, {
  Component,
} from 'react';

import {

    AppRegistry,
    Image,
    StyleSheet,
    DatePickerIOS,
    Text,
    View,
    ActivityIndicatorIOS,
} from 'react-native';

var MOCKED_MOVIES_DATA = [
  {title: 'Title', year: '2015', posters: {thumbnail: 'http://www.52ij.com/uploads/allimg/160317/1110104P8-4.jpg'}},
];

class readDemo extends Component {

  constructor() {
    super();
    this.state = {date: new Date()};
  }

  onDateChange(newDate) {
    this.setState({date: newDate});
  }

  render() {

    return (

      <DatePickerIOS
        date={this.state.date}
        mode='date'

        onDateChange = {this.onDateChange.bind(this)}/>

      );
  }
}


var styles = StyleSheet.create({

  container: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },

  thumbnail: {
    width: 53,
    height: 81,
  },
});

AppRegistry.registerComponent('reactDemo', () => readDemo);

