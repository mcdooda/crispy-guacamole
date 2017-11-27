script {
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
  nodes = {
    {
      name = 'components/detectioncomponent',
    },
    {
      name = 'number',
      loadArguments = {
        3,
      },
    },
    {
      name = 'script/scriptoutput',
      loadArguments = {
        {
          5,
          'Table',
        },
      },
    },
  },
}