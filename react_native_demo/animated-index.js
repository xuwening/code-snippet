/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, {
  AppRegistry,
  Component,
  Animated,
  StyleSheet,
  Text,
  View
} from 'react-native';

class reactDemo extends Component {

  constructor(props) {
    super(props);

    this.state = {
      fadeAnim: new Animated.Value(0),
    };

    this.bounceValue = new Animated.Value(0);
  }

  componentDidMount() {
    Animated.timing(
      this.state.fadeAnim, 
      {toValue: 1, duration: 3000},
      ).start();

    this.bounceValue.setValue(1.5);
    Animated.spring(this.bounceValue, {
      toValue: 0.8,
      friction: 1,
    }).start();
  }

  render() {
    return (
      <View style={styles.container}>

        <Animated.View style={{backgroundColor: 'red', width:100, height: 100, opacity: this.state.fadeAnim}}>
          
        </Animated.View>

        <Animated.View style={{backgroundColor: 'green', width:100, height: 100, transform: [{scale: this.bounceValue},]}}>
        </Animated.View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

AppRegistry.registerComponent('reactDemo', () => reactDemo);
