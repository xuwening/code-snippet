

import React, {
  Component,
} from 'react';

import {

    AppRegistry,
    Image,
    StyleSheet,
    Text,
    View,
} from 'react-native';

var MOCKED_MOVIES_DATA = [
  {title: 'Title', year: '2015', posters: {thumbnail: 'http://www.52ij.com/uploads/allimg/160317/1110104P8-4.jpg'}},
];

class readDemo extends Component {

  render() {

    let movie = MOCKED_MOVIES_DATA[0];

    return (

      <View style={styles.container}>
        <Text>{movie.title}</Text>
        <Text>{movie.year}</Text>
        <Image style={styles.thumbnail} source={{uri: movie.posters.thumbnail}} />
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

