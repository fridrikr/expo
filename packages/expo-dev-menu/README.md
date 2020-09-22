# expo-dev-menu

Expo/React Native module with the developer menu.

# API documentation

- [Documentation for the master branch](https://github.com/expo/expo/blob/master/docs/pages/versions/unversioned/sdk/docs.expo.io.md)
- [Documentation for the latest stable release](https://docs.expo.io/versions/latest/sdk/docs.expo.io/)

# Installation in managed Expo projects

For managed [managed](https://docs.expo.io/versions/latest/introduction/managed-vs-bare/) Expo projects, please follow the installation instructions in the [API documentation for the latest stable release](#api-documentation). If you follow the link and there is no documentation available then this library is not yet usable within managed projects &mdash; it is likely to be included in an upcoming Expo SDK release.

# Installation in bare React Native projects

For bare React Native projects, you must ensure that you have [installed and configured the `react-native-unimodules` package](https://github.com/expo/expo/tree/master/packages/react-native-unimodules) before continuing.

### Add the package to your npm dependencies

```
npm install expo-dev-menu
```

# 💪 Extending the dev-menu's functionalities

One of the main purposes of this package was to provide a easy way to create extensions. We know that developing a react native app can be painful. Often, developers need to create additional tools, which for example, clear the local storage, to test or debug their applications. Some of their work can be integrated with the application itself to save time and make the development more enjoyable.

The below instructions will show you how to create simple extension that removes a key from the `NSUserDefaults`/`SharedPreferences`.

> **Note:** The tutorial was written using Kotlin and Swift. However, you can also use Java and Objective-C if you want.

1.  Create a empty `DevMenuExtension`.

    <details>
    <summary>🤖 Android</summary>

    I. Create a class which extends the `ReactContextBaseJavaModule` and implements the `DevMenuExtensionInterface`.

    ```kotlin
    // CustomDevMenuExtension.kt
    package com.devmenudemo.customdevmenuextension

    import com.facebook.react.bridge.ReactApplicationContext
    import com.facebook.react.bridge.ReactContextBaseJavaModule
    import expo.interfaces.devmenu.DevMenuExtensionInterface
    import expo.interfaces.devmenu.items.DevMenuItem

    class CustomDevMenuExtension(reactContext: ReactApplicationContext)
      : ReactContextBaseJavaModule(reactContext),
        DevMenuExtensionInterface {

        override fun getName() = "CustomDevMenuExtension" // here you can provide name for your extension

        override fun devMenuItems() = emptyList<DevMenuItem>()
    }
    ```

    II. Export created module.

    - Create a react native package class for the extension.

      ```kotlin
      // CustomDevMenuExtensionPackage.kt
      package com.devmenudemo.customdevmenuextension

      import android.view.View
      import com.facebook.react.ReactPackage
      import com.facebook.react.bridge.ReactApplicationContext
      import com.facebook.react.uimanager.ReactShadowNode
      import com.facebook.react.uimanager.ViewManager

      class CustomDevMenuExtensionPackage : ReactPackage {
          override fun createNativeModules(reactContext: ReactApplicationContext) = listOf(
              CustomDevMenuExtension(reactContext) // here you need to export your custom extension
          )

          override fun createViewManagers(reactContext: ReactApplicationContext): List<ViewManager<View, ReactShadowNode<*>>> = listOf()

      }
      ```

    - Go to `MainApplication` (`MainApplication.java` or `MainApplication.kt`) and register created package.

      ```java
      // MainApplication.java

      // You need to import your custom package.
      import com.devmenudemo.customdevmenuextension.CustomDevMenuExtensionPackage; // the package can be different in your case

      ...
      public class MainApplication extends Application implements ReactApplication {
          ...

          @Override
          protected List<ReactPackage> getPackages() {
              @SuppressWarnings("UnnecessaryLocalVariable")
              List<ReactPackage> packages = new PackageList(this).getPackages();

              // Add this line.
              packages.add(new CustomDevMenuExtensionPackage());

              return packages;
          }
      ```

    </details>


    <details>
    <summary>🍏 iOS</summary>

    I. Create a Swift class for your extension.

    ```swift
    // CustomDevMenuExtension.swift
    import EXDevMenuInterface

    @objc(CustomDevMenuExtension)
    open class CustomDevMenuExtension: NSObject, RCTBridgeModule, DevMenuExtensionProtocol {
      public static func moduleName() -> String! {
        return "CustomDevMenuExtension" // here you can provide name for your extension
      }
    }
    ```

    > **Note:** if you don't use Swift in your project earlier, you need to create bridging header. For more information, checks [importing objective-c into swift](https://developer.apple.com/documentation/swift/imported_c_and_objective-c_apis/importing_objective-c_into_swift).

    II. Create a `.m` file to integrate Swift class with react native and add following lines.

    ```objc
    // CustomDevMenuExtension.m

    #import <React/RCTBridgeModule.h>

    @interface RCT_EXTERN_REMAP_MODULE(CustomDevMenuExtensionObjc, CustomDevMenuExtension, NSObject)
    @end
    ```

    III. Add the following line into the bridging header.

    ```objc
    #import <React/RCTBridgeModule.h>
    ```

    </details>

2. Export custom action.

   The main way in which you add functionalities to the dev menu is by exporting actions - functions that will be trigger when the user presses the button.

   - Go to previously created extension class, override the `devMenuItems` method.

     <details>
     <summary>🤖 Android</summary>

     ```kotlin
     // CustomDevMenuExtension.kt
     ...

     // Needed imports
     import android.content.Context.MODE_PRIVATE
     import android.util.Log
     import android.view.KeyEvent
     import expo.interfaces.devmenu.items.DevMenuAction
     import expo.interfaces.devmenu.items.DevMenuItemImportance
     import expo.interfaces.devmenu.items.KeyCommand

     class CustomDevMenuExtension(reactContext: ReactApplicationContext)
       : ReactContextBaseJavaModule(reactContext),
         DevMenuExtensionInterface {
         ...

           override fun devMenuItems(): List<DevMenuItem>? {
               // Firstly, create a function which will be called when the user presses the button.
               val clearSharedPreferencesOnPress: () -> Unit = {
                   reactApplicationContext
                     .getSharedPreferences("your.shared.preferences", MODE_PRIVATE)
                     .edit()
                     .apply {
                         remove("key_to_remove")
                         Log.i("CustomDevMenuExt", "Remove key from SharedPreferences")
                         apply()
                     }
               }

               // Then, create `DevMenuAction` object.
               val clearSharedPreferences = DevMenuAction(
                   actionId = "clear_shared_preferences", // This string identifies your custom action. Make sure that it's unique.
                   action = clearSharedPreferencesOnPress
               ).apply {
                   label = { "Clear shared preferences" } // This string will be displayed in the dev menu.
                   glyphName = { "delete" } // This is a icon name used to present your action. You can use any icon from the `MaterialCommunityIcons`.
                   importance = DevMenuItemImportance.HIGH.value // This value tells the dev-menu in which order the actions should be rendered.
                   keyCommand = KeyCommand(KeyEvent.KEYCODE_S) // You can associate key commend with your action.
               }

               // Return created object. Note: you can register multiple actions if you want.
               return listOf(clearSharedPreferences)
           }
       }
     ```

      </details>

      <details>
      <summary>🍏 iOS</summary>

     ```swift
     // CustomDevMenuExtension.swift
     ...

     @objc(CustomDevMenuExtension)
     open class CustomDevMenuExtension: NSObject, RCTBridgeModule, DevMenuExtensionProtocol {

        @objc
        open func devMenuItems() -> [DevMenuItem]? {
            // Firstly, create a function which will be called when the user presses the button.
            let clearNSUserDefaultsOnPress = {
              let prefs = UserDefaults.standard
              prefs.removeObject(forKey: "key_to_remove")
            }

            let clearNSUserDefaults = DevMenuAction(
                withId: "clear_nsusersdefaults", // This string identifies your custom action. Make sure that it's unique.
                action: clearNSUserDefaultsOnPress
            )

            clearNSUserDefaults.label = { "Clear NSUserDefaults" } // This string will be displayed in the dev menu.
            clearNSUserDefaults.glyphName = { "delete" } // This is a icon name used to present your action. You can use any icon from the `MaterialCommunityIcons`.
            clearNSUserDefaults.importance = DevMenuItem.ImportanceHigh // This value tells the dev-menu in which order the actions should be rendered.
            clearNSUserDefaults.registerKeyCommand(input: "p", modifiers: .command) // You can associate key commend with your action.

            // Return created object. Note: you can register multiple actions if you want.
            return [clearNSUserDefaults]
        }
     }
     ```

      </details>

After all those steps you should see something like this:

![Final result](custom_dev_menu_extension_example.png)

# 📚 API

```js
import * as DevMenu from 'expo-dev-menu';
```

For now, the `DevMenu` module exports only one method - [`openMenu`](#openmenu).

### openMenu()

Opens the dev menu.

#### Example

Using this method you can create a simple button that will open the `dev-menu`.

```js
import * as DevMenu from 'expo-dev-menu';
import { Button } from 'react-native';

export const DevMenuButton = () => (
  <Button
    onPress={() => {
      DevMenu.openMenu();
    }}
    title="Press to open the dev menu 🚀"
    color="#841584"
  />
);
```

# Contributing

Contributions are very welcome! Please refer to guidelines described in the [contributing guide](https://github.com/expo/expo#contributing).
