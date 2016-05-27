

import React, {
  Component,
} from 'react';

import {

    AppRegistry,
    Image,
    StyleSheet,
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
    this.state = {animating: true};
  }

  componentWillMount() {

    setTimeout(
        () => {
          this.setState({animating: !this.state.animating});
        }, 3000
      );
  }

  render() {

    let movie = MOCKED_MOVIES_DATA[0];

    return (

      <View style={styles.container}>
          <ActivityIndicatorIOS  animating={this.state.animating} color="#0000ff" />
          <ActivityIndicatorIOS color="#aa00aa" />
          <ActivityIndicatorIOS color="#aa3300" />
          <ActivityIndicatorIOS color="#00aa00" />
      </View>

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

