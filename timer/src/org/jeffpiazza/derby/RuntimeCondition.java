package org.jeffpiazza.derby;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.logging.Level;
import java.util.logging.Logger;

// Some timer profile behaviors have to change in response when a flag changes or
// the environment is similarly changed.  To accommodate these, the profile can
// include a RuntimeCondition name in some places, and the corresponding
// RuntimeCondition method gets called to make a decision.
//
// Each RuntimeCondition has to have a corresponding implementation in javascript.
public class RuntimeCondition {
  public static boolean evaluate(String method_name) {
    // Conditions are often optional; if one's not provided, then assume the
    // operation should just go forward.
    if (method_name == null) {
      return true;
    }
    try {
      Class rc_class = Class.forName("org.jeffpiazza.derby.RuntimeCondition");
      Method m = rc_class.getMethod(method_name, new Class[]{});
      return (boolean) m.invoke(null, new Object[]{});
    } catch (ReflectiveOperationException ex) {
    } catch (SecurityException ex) {
    } catch (IllegalArgumentException ex) {
    }
    return false;
  }

  //////////////////////////////////////////////////////////////////////////////
  // The methods below here need to have equivalent implementations for
  // javascript.
  //////////////////////////////////////////////////////////////////////////////

  public static boolean fasttrack_poll_mark() {
    // The automatic gate release on a FastTrack timer re-uses
    // the laser reset line; sending an LR command will cause the
    // state gate to open.
    //
    // no-gate-watcher means we wouldn't be able to tell when to stop
    // sending the laser-reset commands.
    return !Flag.fasttrack_automatic_gate_release.value()
        && !Flag.no_gate_watcher.value();
  }
  public static boolean fasttrack_has_automatic_gate_release() {
    return Flag.fasttrack_automatic_gate_release.value();
  }
}