[@bs.module "@material-ui/core"]
external useMediaQuery: string => bool = "useMediaQuery";

let useIsDesktop = () => {
  useMediaQuery("(min-width: 600px)");
};
