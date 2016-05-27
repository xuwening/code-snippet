
import React, {Component} from 'react';
import {

	View,
	StyleSheet,
	Image,
	Alert,
} from 'react-native';

const SideMenu = require('react-native-side-menu');

const Menu = require('./Menu');
const SecondView = require('./View/SecondView');
const Button = require('./View/Button');
const TabItems = require('./TabItems');


var styles = StyleSheet.create({

  navigatorBar: {
    // flex:1,
    // flexDirection: 'column',
    backgroundColor: 'deepskyblue',
    height: 64,
  },

  button: {
    // position: 'absolute',
    top: 20,
    padding: 10,
    // resizeMode: 'contain',
    width: 60,
    
  },

  rightButton: {
    position: 'absolute',
    top: 30,
    right: 10,
  },

  centerButton: {
    position: 'absolute',
    left: 100,
    top: 20,
  },
});

class TabFrame extends Component {

  constructor(props) {
    super(props);

    this.state = {
      isOpen: false,
      selectedItem: 'About',
    };
  }

  toggle() {
    this.setState({
      isOpen: !this.state.isOpen,
    });
  }

  updateMenuState(isOpen) {
    this.setState({ isOpen, });
  }

  onMenuItemSelected = (item) => {
    this.setState({
      isOpen: false,
      selectedItem: item,
    });
  }

  render() {
    const menu = <Menu onItemSelected={this.onMenuItemSelected} nav={this.props.nav}/>;

    return (
      <SideMenu
        menu={menu}
        isOpen={this.state.isOpen}
        onChange={(isOpen) => this.updateMenuState(isOpen)}>


        <View style={styles.navigatorBar}>
          

            <Button style={styles.button} onPress={() => this.toggle()}>
              <Image source={require('../res/top/home.png')} />
            </Button>

            <Button style={styles.centerButton} onPress={()=>Alert.alert('', 'title.')}>
              <Image source={require('../res/top/title.png')} />
            </Button>

            <Button style={styles.rightButton} backgroundColor='red' onPress={()=> this.props.nav.push({component: SecondView})}>
              <Image source={require('../res/top/message.png')}  />
            </Button>

          

        </View>

          <TabItems nav={this.props.nav}/>


      </SideMenu>
    );
  }
};


module.exports = TabFrame;

