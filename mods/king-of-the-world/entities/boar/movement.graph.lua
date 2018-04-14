script {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      3,
      1,
      1,
      1,
    },
    {
      4,
      1,
      1,
      2,
    },
    {
      5,
      1,
      1,
      3,
    },
    {
      6,
      1,
      1,
      4,
    },
  },
  nodes = {
    {
      name = 'components/movementcomponent',
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
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        3,
      },
      name = 'number',
    },
    {
      loadArguments = {
        7,
      },
      name = 'number',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
  },
}