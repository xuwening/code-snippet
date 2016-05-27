

import React, {
  Component,
} from 'react';

import {

    AppRegistry,
    Image,
    StyleSheet,
    Text,
    View,
    Switch,
    TouchableOpacity,
    Navigator,
    TabBarIOS,
} from 'react-native';


class FlexBox extends Component {

  constructor() {
    super();

    this.state = {selected: true};
  }

  valueChanged(val) {
    this.setState({selected: val});
  }

  render() {

    return ( 

    <View style={styles.container}>
        <View style={styles.item}>
        </View>
        <View style={styles.item}>
        </View>
        <View style={styles.item}>
        </View>
        <Switch onValueChange={this.valueChanged.bind(this)} value={this.state.selected} onTintColor='red' thumbTintColor='yellow' tintColor='blue'>
        </Switch>
    </View>

    );
  }
}


var ss = {

  container: {
    flex: 1,
    flexDirection: 'column',
    flexWrap: 'wrap',
    justifyContent: 'center',
    alignItems: 'center',
    // alignSelf: 'flex-start',
    // margin: 20,
    padding: 5,
    backgroundColor: 'gray',
  },

  item: {

    // flex: 1,
    // alignSelf: 'center',
    margin: 5,
    width: 100,
    height: 300,
    backgroundColor: 'white',
  },

};
var styles = StyleSheet.create(ss);

AppRegistry.registerComponent('reactDemo', () => FlexBox);

