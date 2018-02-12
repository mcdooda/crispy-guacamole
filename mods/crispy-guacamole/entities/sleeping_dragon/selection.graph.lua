script {
  nodes = {
    {
      name = 'components/selectioncomponent',
    },
    {
      loadArguments = {
        false,
      },
      name = 'bool',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
  },
  links = {
    {
      1,
      1,
      3,
      1,
    },
    {
      2,
      1,
      1,
      1,
    },
  },
}