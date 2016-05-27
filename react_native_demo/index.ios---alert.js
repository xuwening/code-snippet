

'use strict'


var platform = require('Platform');
var React = require('react-native');

var {
  AppRegistry,
  StyleSheet,
  Text,
  View,
  Image,
  Alert,
  TouchableOpacity,
} = React;


var styles = StyleSheet.create({

  view1: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'flex-start',
    alignItems: 'center',
    backgroundColor: 'blue',
  },

  view2: {
    flex: 1,
    width:100,
    height:200,
    margin: 10,
    backgroundColor: 'red',
    borderTopColor: 'white',
    borderRightColor: 'yellow',
    borderWidth: 2,

    borderTopLeftRadius: 20,
    borderBottomRightRadius:20,
  },

  button: {
    flex: 1,
    width:100,
    height:200,
  },

});

// var AwesomeProject1 = React.createClass({

//   render() {

//     return (
//         <View style={ styles.view1 }>

//           <View style={styles.view2}>
//           </View>

//           <View style={styles.view2}>
//           </View>
//         </View>
//       );
//   },

// });

class AwesomeProject extends React.Component {


  _alertOKActon() {

    console.log('aaaaa');
  }

  _onPressButton() {
    Alert.alert('title'
      , 'message.....'
      // , [{text:'cancel', onPress: () => console.log('press ok button .......')}]
      , [{text:'cancel', onPress: this._alertOKActon},]
      );
  }

  render() {

    return (
        <View style={ styles.view1 }>

          <View style={styles.view2}>
          </View>

          <View style={styles.view2}>
          </View>

          <View style={styles.view2}>
          </View>
          <TouchableOpacity onPress={this._onPressButton}>

            <Image style={styles.button} source={{uri:'http://www.52ij.com/uploads/allimg/160317/1110104P8-4.jpg'}}/>

          </TouchableOpacity>
        </View>
      );
  }
}


AppRegistry.registerComponent('AwesomeProject', ()=> AwesomeProject );



